#include "thread.h"
#include "log.h"
#include "util.h"

#include <stdexcept>

namespace jaysheng {

static jaysheng::Logger::ptr g_logger = JAYSHENG_LOG_NAME("system");

// 线程局部变量
static thread_local Thread *t_thread = nullptr;
static thread_local std::string t_thread_name = "UNKNOW";

// static function

Thread *Thread::GetThis() {
    return t_thread;
}

std::string Thread::GetName() {
    return t_thread_name;
}

void Thread::SetName(const std::string &name) {
    t_thread_name = name;
}

void *Thread::run(void *arg) {
    Thread *thread = (Thread *)arg;
    t_thread = thread;
    t_thread_name = thread->m_name;
    thread->m_id = jaysheng::GetThreadId();
    pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());

    std::function<void()> cb;
    cb.swap(thread->m_cb);

    thread->m_semaphore.notify();

    cb();
    return 0;
}

Thread::Thread(std::function<void()> cb, const std::string &name)
    : m_id(-1),
      m_name(name),
      m_thread(0),
      m_cb(cb) {
    if (name.empty()) {
        m_name = "UNKNOW";
    }
    int rt = pthread_create(&m_thread, nullptr, Thread::run, this);
    if (rt) {
        JAYSHENG_LOG_ERROR(g_logger) << "pthread_create thread failed, rt=" << rt
                                     << " name=" << m_name;
        throw std::logic_error("pthread_create error.");
    }
    m_semaphore.wait();
}

Thread::~Thread() {
    if (m_thread) {
        pthread_detach(m_thread);
    }
}

void Thread::join() {
    if (m_thread) {
        int rt = pthread_join(m_thread, nullptr);
        if (rt) {
            JAYSHENG_LOG_ERROR(g_logger) << "pthread_join thread failed, rt=" << rt
                                         << " name=" << m_name;
            throw std::logic_error("pthread_join error.");
        }
        m_thread = 0;
    }
}

} // namespace jaysheng
