#ifndef __JAYSHENG_THREAD_H__
#define __JAYSHENG_THREAD_H__

#include "base/noncopy.h"
#include "mutex.h"

#include <functional>
#include <memory>
#include <pthread.h>

namespace jaysheng {

class Thread : Noncopy {
public:
    using ptr = std::shared_ptr<Thread>;
    Thread(std::function<void()> cb, const std::string &name);
    ~Thread();

    pid_t id() const { return m_id; }
    std::string name() const { return m_name; }

    void join();

    // 获得当前线程
    static Thread *GetThis();
    // 获得当前线程id
    static std::string GetName();
    // 设置当前线程名称
    static void SetName(const std::string &name);

protected:
    static void *run(void *arg);

private:
    pid_t m_id;         // 线程id
    std::string m_name; // 线程名称
    pthread_t m_thread;
    std::function<void()> m_cb; // 执行函数
    Semaphore m_semaphore;      // 信号量
};

} // namespace jaysheng

#endif // !__JAYSHENG_THREAD_H__