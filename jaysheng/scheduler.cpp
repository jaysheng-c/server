#include "scheduler.h"
#include "log.h"
#include "macro.h"
#include "util.h"

namespace jaysheng {

static jaysheng::Logger::ptr g_logger = JAYSHENG_LOG_NAME("system");

static thread_local Scheduler *t_scheduler = nullptr;
static thread_local Fiber *t_scheduler_fiber = nullptr;

// static

Scheduler *Scheduler::GetThis() {
    return t_scheduler;
}

Fiber *Scheduler::GetMainFiber() {
    return t_scheduler_fiber;
}

Scheduler::Scheduler(size_t thread_count, bool use_caller, const std::string &name)
    : m_active_thread_count(0),
      m_idle_thread_count(0),
      m_stop(true),
      m_auto_stop(false),
      m_name(name) {
    JAYSHENG_ASSERT(thread_count > 0);
    if (use_caller) {
        // jaysheng::Fiber::GetThis();
        --thread_count;

        JAYSHENG_ASSERT(GetThis() == nullptr);
        t_scheduler = this;
        m_root_fiber.reset(new Fiber(std::bind(&Scheduler::run, this), 0, true));
        jaysheng::Thread::SetName(m_name);

        t_scheduler_fiber = m_root_fiber.get();
        m_root_threadId = jaysheng::GetThreadId();
        m_threadIds.push_back(m_root_threadId);
    } else {
        m_root_threadId = -1;
    }
    m_thread_count = thread_count;
}

Scheduler::~Scheduler() {
    JAYSHENG_ASSERT(m_stop);
    if (GetThis() == this) {
        t_scheduler = nullptr;
    }
}

void Scheduler::start() {
    MutexType::Lock lock(m_mutex);
    if (!m_stop) {
        return;
    }
    m_stop = false;
    JAYSHENG_ASSERT(m_threads.empty());

    m_threads.resize(m_thread_count);
    for (size_t i = 0; i < m_thread_count; ++i) {
        m_threads[i].reset(new Thread(std::bind(&Scheduler::run, this),
                                      m_name + "_" + std::to_string(i)));
        m_threadIds.push_back(m_threads[i]->id());
    }
}

void Scheduler::stop() {
    JAYSHENG_LOG_DEBUG(g_logger) << m_name << " stop";
    m_auto_stop = true;
    if (m_root_fiber &&
        m_thread_count == 0 &&
        (m_root_fiber->state() == Fiber::TERM ||
         m_root_fiber->state() == Fiber::INIT)) {
        JAYSHENG_LOG_INFO(g_logger) << this << " stopped.";
        m_stop = true;
        if (isStopping()) {
            return;
        }
    }

    if (m_root_threadId != -1) {
        JAYSHENG_ASSERT(GetThis() == this);
    } else {
        JAYSHENG_ASSERT(GetThis() != this);
    }

    m_stop = true;
    for (size_t i = 0; i < m_thread_count; ++i) {
        tickle();
    }
    if (m_root_fiber) {
        tickle();
        if (!isStopping()) {
            m_root_fiber->call();
        }
    }

    std::vector<Thread::ptr> thrs;
    {
        MutexType::Lock lock(m_mutex);
        thrs.swap(m_threads);
    }
    for (auto &i : thrs) {
        i->join();
    }
}
void Scheduler::switchTo(int thread) {
    JAYSHENG_ASSERT(GetThis() != nullptr);
    if (GetThis() == this) {
        if (thread == -1 || thread == jaysheng::GetThreadId()) {
            return;
        }
    }
    scheduler(Fiber::GetThis(), thread);
    Fiber::YieldToHold();
}

std::ostream &Scheduler::dump(std::ostream &os) {
    os << "[Scheduler name=" << m_name
       << " size=" << m_thread_count
       << " active_count=" << m_active_thread_count
       << " idle_count=" << m_idle_thread_count
       << " stopping=" << m_stop
       << " ]" << std::endl
       << "    ";
    for (size_t i = 0; i < m_threadIds.size(); ++i) {
        if (i) {
            os << ", ";
        }
        os << m_threadIds[i];
    }
    return os;
}

void Scheduler::tickle() {
    JAYSHENG_LOG_INFO(g_logger) << "tickle.";
}

bool Scheduler::isStopping() {
    MutexType::Lock lock(m_mutex);
    return m_auto_stop && m_stop &&
           m_fibers.empty() && m_active_thread_count == 0;
}

void Scheduler::idle() {
    JAYSHENG_LOG_INFO(g_logger) << "idle";
    while (!m_stop) {
        JAYSHENG_LOG_INFO(g_logger) << "in loop";
        Fiber::YieldToHold();
    }
}

void Scheduler::run() {
    JAYSHENG_LOG_DEBUG(g_logger) << m_name << " run";
    // set_hook_enable(true);
    setThis();
    if (jaysheng::GetThreadId() != m_root_threadId) {
        t_scheduler_fiber = Fiber::GetThis().get();
    }

    Fiber::ptr idle_fiber(new Fiber(std::bind(&Scheduler::idle, this)));
    Fiber::ptr cb_fiber;

    FiberAndThread ft;
    while (true) {
        ft.reset();
        bool tickle_me = false;
        bool is_active = false;
        {
            MutexType::Lock lock(m_mutex);
            auto it = m_fibers.begin();
            while (it != m_fibers.end()) {
                if (it->thread != -1 && it->thread != jaysheng::GetThreadId()) {
                    ++it;
                    tickle_me = true;
                    continue;
                }

                JAYSHENG_ASSERT(it->fiber || it->cb);
                if (it->fiber && it->fiber->state() == Fiber::EXEC) {
                    ++it;
                    continue;
                }

                ft = *it;
                m_fibers.erase(it++);
                ++m_active_thread_count;
                is_active = true;
                break;
            }
            tickle_me |= it != m_fibers.end();
        }

        if (tickle_me) {
            tickle();
        }

        if (ft.fiber && (ft.fiber->state() != Fiber::TERM && ft.fiber->state() != Fiber::EXCEPT)) {
            ft.fiber->swapIn();
            --m_active_thread_count;

            if (ft.fiber->state() == Fiber::READY) {
                scheduler(ft.fiber);
            } else if (ft.fiber->state() != Fiber::TERM && ft.fiber->state() != Fiber::EXCEPT) {
                // ft.fiber->m_state = Fiber::HOLD;
                ft.fiber->setState(Fiber::HOLD);
            }
            ft.reset();
        } else if (ft.cb) {
            if (cb_fiber) {
                cb_fiber->reset(ft.cb);
            } else {
                cb_fiber.reset(new Fiber(ft.cb));
            }
            ft.reset();
            cb_fiber->swapIn();
            --m_active_thread_count;
            if (cb_fiber->state() == Fiber::READY) {
                scheduler(cb_fiber);
                cb_fiber.reset();
            } else if (cb_fiber->state() == Fiber::EXCEPT || cb_fiber->state() == Fiber::TERM) {
                cb_fiber->reset(nullptr);
            } else { //if(cb_fiber->getState() != Fiber::TERM) {
                // cb_fiber->m_state = Fiber::HOLD;
                ft.fiber->setState(Fiber::HOLD);
                cb_fiber.reset();
            }
        } else {
            if (is_active) {
                --m_active_thread_count;
                continue;
            }
            if (idle_fiber->state() == Fiber::TERM) {
                JAYSHENG_LOG_INFO(g_logger) << "idle fiber term";
                break;
            }

            ++m_idle_thread_count;
            idle_fiber->swapIn();
            --m_idle_thread_count;
            if (idle_fiber->state() != Fiber::TERM && idle_fiber->state() != Fiber::EXCEPT) {
                // idle_fiber->m_state = Fiber::HOLD;
                ft.fiber->setState(Fiber::HOLD);
            }
        }
    }
}

void Scheduler::setThis() {
    t_scheduler = this;
}

} // namespace jaysheng
