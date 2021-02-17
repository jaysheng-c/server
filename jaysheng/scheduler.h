#ifndef __JAYSHENG_SCHEDULER_H__
#define __JAYSHENG_SCHEDULER_H__

#include "fiber.h"
#include "mutex.h"
#include "thread.h"

#include <atomic>
#include <functional>
#include <list>
#include <memory>
#include <vector>

namespace jaysheng {

class Scheduler {
public:
    using ptr = std::shared_ptr<Scheduler>;
    using MutexType = Mutex;

public:
    template <class FiberOrCb>
    void scheduler(FiberOrCb fc, int thread = -1) {
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            need_tickle = scheduleNoLock(fc, thread) || need_tickle;
        }
        if (need_tickle) {
            tickle();
        }
    }

    template <class InputIterator>
    void scheduler(InputIterator begin, InputIterator end) {
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            while (begin != end) {
                need_tickle = scheduleNoLock(&*begin, -1) || need_tickle;
                ++begin;
            }
        }
        if (need_tickle) {
            tickle();
        }
    }

private:
    template <class FiberOrCb>
    bool scheduleNoLock(FiberOrCb fc, int thread) {
        bool need_tickle = m_fibers.empty();
        FiberAndThread ft(fc, thread);
        if (ft.fiber || ft.cb) {
            m_fibers.push_back(ft);
        }
        return need_tickle;
    }

public:
    Scheduler(size_t thread_count = 1, bool use_caller = true, const std::string &name = "");
    ~Scheduler();

    const std::string &name() const { return m_name; }
    void start();
    void stop();
    void switchTo(int thread = -1);
    std::ostream &dump(std::ostream &os);

public:
    static Scheduler *GetThis();
    static Fiber *GetMainFiber();

protected:
    /**
     * @brief 通知协程调度器有任务了
     */
    virtual void tickle();

    virtual bool isStopping();

    /**
     * @brief 协程无任务可调度时执行idle协程
     */
    virtual void idle();

    /**
     * @brief 协程调度函数
     */
    void run();

    /**
     * @brief 设置当前的协程调度器
     */
    void setThis();

    bool hasIdleThreads() { return m_idle_thread_count > 0; }

private:
    struct FiberAndThread {
        FiberAndThread(Fiber::ptr f, int thr)
            : fiber(f), cb(nullptr), thread(thr) {
        }

        FiberAndThread(Fiber::ptr *f, int thr)
            : cb(nullptr), thread(thr) {
            fiber.swap(*f);
        }

        FiberAndThread(std::function<void()> f, int thr)
            : fiber(nullptr), cb(f), thread(thr) {
        }

        FiberAndThread(std::function<void()> *f, int thr)
            : fiber(nullptr), thread(thr) {
            cb.swap(*f);
        }

        FiberAndThread()
            : fiber(nullptr), cb(nullptr), thread(-1) {
        }

        void reset() {
            fiber = nullptr;
            cb = nullptr;
            thread = -1;
        }

        Fiber::ptr fiber;
        std::function<void()> cb;
        int thread;
    };

protected:
    std::vector<int> m_threadIds;              // 协程下的线程id
    size_t m_thread_count;                     // 线程数量
    std::atomic<size_t> m_active_thread_count; // 活动线程个数
    std::atomic<size_t> m_idle_thread_count;   // 空闲线程个数

    bool m_stop;         //  是否停止
    bool m_auto_stop;    //  是否自动停止
    int m_root_threadId; // 主线程id
private:
    MutexType m_mutex;
    std::vector<Thread::ptr> m_threads;
    std::list<FiberAndThread> m_fibers;
    Fiber::ptr m_root_fiber; // use_caller为true时有效, 调度协程
    std::string m_name;      // Scheduler名称
};

} // namespace jaysheng

#endif // !__JAYSHENG_SCHEDULER_H__