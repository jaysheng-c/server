#include "fiber.h"
#include "log.h"
#include "macro.h"

#include <atomic>

namespace jaysheng {

static jaysheng::Logger::ptr g_logger = JAYSHENG_LOG_NAME("system");

static std::atomic<uint64_t> s_fiber_id{0};
static std::atomic<uint64_t> s_fiber_count{0};

static uint32_t g_fiber_stack_size = 128 * 1024;

// 线程局部变量
static thread_local Fiber *t_fiber = nullptr;
static thread_local Fiber::ptr t_threadFiber = nullptr;

// 分配器
class MallocStackAllocator {
public:
    static void *Alloc(size_t size) {
        return malloc(size);
    }
    static void Dealloc(void *p, size_t size) {
        return free(p);
    }
};
using StackAllocator = MallocStackAllocator;

// static

void Fiber::SetThis(Fiber *f) {
    t_fiber = f;
}

Fiber::ptr Fiber::GetThis() {
    if (t_fiber) {
        return t_fiber->shared_from_this();
    }
    Fiber::ptr main_fiber(new Fiber);
    JAYSHENG_ASSERT(t_fiber == main_fiber.get());
    t_threadFiber = main_fiber;
    return t_fiber->shared_from_this();
}

uint64_t Fiber::GetFiberId() {
    if (t_fiber) {
        return t_fiber->id();
    }
    return 0;
}

void Fiber::YieldToReady() {
    Fiber::ptr cur = GetThis();
    JAYSHENG_ASSERT(cur->m_state == EXEC);
    cur->m_state = READY;
    cur->swapOut();
}

void Fiber::YieldToHold() {
    Fiber::ptr cur = GetThis();
    JAYSHENG_LOG_DEBUG(g_logger) << "in YieldToHold, fiber_id=" << std::to_string(cur->id());
    JAYSHENG_ASSERT(cur->m_state == EXEC);
    // cur->m_state = HOLD;
    cur->swapOut();
}

uint64_t Fiber::TotalFibers() {
    return s_fiber_count;
}

void Fiber::MainFunc() {
    Fiber::ptr cur = GetThis();
    JAYSHENG_ASSERT(cur);
    try {
        cur->m_cb();
        cur->m_cb = nullptr;
        cur->m_state = TERM;
    } catch (std::exception &e) {
        cur->m_state = EXCEPT;
        JAYSHENG_LOG_ERROR(g_logger) << "Fiber Except: " << e.what()
                                     << std::endl
                                     << jaysheng::BacktraceToString();
    } catch (...) {
        cur->m_state = EXCEPT;
        JAYSHENG_LOG_ERROR(g_logger) << "Fiber Except: fiber_id=" << cur->id()
                                     << std::endl
                                     << jaysheng::BacktraceToString();
    }

    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->swapOut();
    JAYSHENG_ASSERT2(false, "never reach fiber_id=" + std::to_string(raw_ptr->id()));
}

void Fiber::CallerMainFunc() {
    Fiber::ptr cur = GetThis();
    JAYSHENG_ASSERT(cur);
    try {
        cur->m_cb();
        cur->m_cb = nullptr;
        cur->m_state = TERM;
    } catch (std::exception &ex) {
        cur->m_state = EXCEPT;
        JAYSHENG_LOG_ERROR(g_logger) << "Fiber Except: " << ex.what()
                                     << " fiber_id=" << cur->id()
                                     << std::endl
                                     << jaysheng::BacktraceToString();
    } catch (...) {
        cur->m_state = EXCEPT;
        JAYSHENG_LOG_ERROR(g_logger) << "Fiber Except"
                                     << " fiber_id=" << cur->id()
                                     << std::endl
                                     << jaysheng::BacktraceToString();
    }

    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->back();
    JAYSHENG_ASSERT2(false, "never reach fiber_id=" + std::to_string(raw_ptr->id()));
}

// private

Fiber::Fiber() {
    m_state = EXEC;
    SetThis(this);

    if (getcontext(&m_ctx)) {
        JAYSHENG_ASSERT2(false, "getcontex error");
    }

    // 协程计数加一
    ++s_fiber_count;

    JAYSHENG_LOG_DEBUG(g_logger) << "Fiber::Fiber main.";
}

// public

Fiber::Fiber(std::function<void()> cb, uint32_t stackSize, bool use_caller)
    : m_id(++s_fiber_id),
      m_state(INIT),
      m_cb(cb) {
    ++s_fiber_count;
    m_stackSize = stackSize ? stackSize : g_fiber_stack_size;

    m_stack = StackAllocator::Alloc(m_stackSize);

    if (getcontext(&m_ctx)) {
        JAYSHENG_ASSERT2(false, "getcontex error");
    }
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stackSize;

    if (!use_caller) {
        makecontext(&m_ctx, &Fiber::MainFunc, 0);
    } else {
        makecontext(&m_ctx, &Fiber::CallerMainFunc, 0);
    }
    JAYSHENG_LOG_DEBUG(g_logger) << "Fiber::Fiber id=" << m_id;
}

Fiber::~Fiber() {
    --s_fiber_count;
    if (m_stack) {
        JAYSHENG_ASSERT(m_state == TERM || m_state == EXCEPT || m_state == INIT);
        StackAllocator::Dealloc(m_stack, m_stackSize);
    } else {
        JAYSHENG_ASSERT(!m_cb);
        JAYSHENG_ASSERT(m_state == EXEC);

        Fiber *cur = t_fiber;
        if (cur == this) {
            SetThis(nullptr);
        }
    }
    JAYSHENG_LOG_DEBUG(g_logger) << "Fiber::~Fiber id=" << m_id
                                 << " total=" << s_fiber_count;
}

void Fiber::reset(std::function<void()> cb) {
    JAYSHENG_ASSERT(m_stack);
    JAYSHENG_ASSERT(m_state == TERM || m_state == EXCEPT || m_state == INIT);
    m_cb = cb;
    if (getcontext(&m_ctx)) {
        JAYSHENG_ASSERT2(false, "getcontex error");
    }
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stackSize;
    makecontext(&m_ctx, &Fiber::MainFunc, 0);
    m_state = INIT;
}

void Fiber::swapIn() {
    SetThis(this);
    JAYSHENG_ASSERT(m_state != EXEC);
    m_state = EXEC;
    if (swapcontext(&t_threadFiber->m_ctx, &m_ctx)) {
        JAYSHENG_ASSERT2(false, "swapcontext error.");
    }
}

void Fiber::swapOut() {
    SetThis(t_threadFiber.get());
    if (swapcontext(&m_ctx, &t_threadFiber->m_ctx)) {
        JAYSHENG_ASSERT2(false, "swapcontext error.");
    }
}

void Fiber::call() {
    SetThis(this);
    m_state = EXEC;
    if (swapcontext(&t_threadFiber->m_ctx, &m_ctx)) {
        JAYSHENG_ASSERT2(false, "swapcontext error.");
    }
}

void Fiber::back() {
    SetThis(t_threadFiber.get());
    if (swapcontext(&m_ctx, &t_threadFiber->m_ctx)) {
        JAYSHENG_ASSERT2(false, "swapcontext error.");
    }
}

} // namespace jaysheng
