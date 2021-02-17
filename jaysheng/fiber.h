#ifndef __JAYSHENG_FIBER_H__
#define __JAYSHENG_FIBER_H__

#include "fiber.h"

#include <functional>
#include <memory>
#include <stdint.h>
#include <ucontext.h>

namespace jaysheng {

class Fiber : public std::enable_shared_from_this<Fiber> {
public:
    using ptr = std::shared_ptr<Fiber>;
    enum STATE {
        INIT,  // 初始化状态
        HOLD,  // 暂停状态
        EXEC,  // 执行中状态
        TERM,  // 结束状态
        READY, // 可执行状态
        EXCEPT // 异常状态
    };

private:
    // 不允许外部使用默认构造，由自身static函数调用
    Fiber();

public:
    /**
     * @brief 构造函数
     * @param[in] cb 协程执行的函数
     * @param[in] stacksize 协程栈大小
     * @param[in] use_caller 是否在MainFiber上调度
     */
    Fiber(std::function<void()> cb, uint32_t stackSize = 0, bool use_caller = false);
    ~Fiber();

    uint64_t id() const { return m_id; }
    STATE state() const { return m_state; }
    void setState(STATE val) { m_state = val; }

    void reset(std::function<void()> cb);

    /**
     * @brief 将当前线程切换到执行状态
     * @pre 执行的为当前线程的主协程
     */
    void swapIn();

    // 将当前协程切换到后台
    void swapOut();

    /**
     * @brief 将当前线程切换到执行状态
     * @pre 执行的为当前线程的主协程
     */
    void call();

    /**
     * @brief 将当前线程切换到后台
     * @pre 执行的为该协程
     * @post 返回到线程的主协程
     */
    void back();

    static void SetThis(Fiber *f);
    static Fiber::ptr GetThis();
    static uint64_t GetFiberId();

    /**
     * @brief 将当前协程切换到后台,并设置为READY状态
     * @post getState() = READY
     */
    static void YieldToReady();

    /**
     * @brief 将当前协程切换到后台,并设置为HOLD状态
     * @post getState() = HOLD
     */
    static void YieldToHold();

    // 返回当前协程总数
    static uint64_t TotalFibers();

    /**
     * @brief 协程执行函数
     * @post 执行完成返回到线程主协程
     */
    static void MainFunc();

    /**
     * @brief 协程执行函数
     * @post 执行完成返回到线程调度协程
     */
    static void CallerMainFunc();

private:
    uint64_t m_id;        // 协程id
    uint32_t m_stackSize; // 运行栈大小
    STATE m_state;        // 状态
    ucontext_t m_ctx;     // 上下文
    void *m_stack;        // 运行栈指针
    std::function<void()> m_cb;
};

} // namespace jaysheng

#endif // !__JAYSHENG_FIBER_H__