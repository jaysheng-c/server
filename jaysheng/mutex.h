#ifndef __JAYSHENG_MUTEX_H__
#define __JAYSHENG_MUTEX_H__

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

#include "base/noncopy.h"

namespace jaysheng {

// 信号量
class Semaphore : Noncopy {
public:
    Semaphore(uint32_t count = 0);
    ~Semaphore();
    void wait();
    void notify();

private:
    sem_t m_semaphore;
};

// 互斥锁模板，实现锁的自动释放
template <class T>
class ScopedLock {
public:
    ScopedLock(T &mutex)
        : m_mutex(mutex) {
        m_mutex.lock();
        m_isLock = true;
    }
    ~ScopedLock() {
        m_mutex.unlock();
        m_isLock = false;
    }

    void lock() {
        if (!m_isLock) {
            m_mutex.lock();
            m_isLock = true;
        }
    }
    void unlock() {
        if (m_isLock) {
            m_mutex.unlock();
            m_isLock = false;
        }
    }

private:
    T &m_mutex;
    bool m_isLock;
};

// 读锁模板
template <class T>
class ReadScopedLock {
public:
    ReadScopedLock(T &mutex)
        : m_mutex(mutex) {
        m_mutex.rdlock();
        m_isLock = true;
    }
    ~ReadScopedLock() {
        m_mutex.unlock();
        m_isLock = false;
    }

    void lock() {
        if (!m_isLock) {
            m_mutex.rdlock();
            m_isLock = true;
        }
    }
    void unlock() {
        if (m_isLock) {
            m_mutex.unlock();
            m_isLock = false;
        }
    }

private:
    T &m_mutex;
    bool m_isLock;
};

// 写锁模板
template <class T>
class WriteScopedLock {
public:
    WriteScopedLock(T &mutex)
        : m_mutex(mutex) {
        m_mutex.wrlock();
        m_isLock = true;
    }
    ~WriteScopedLock() {
        m_mutex.unlock();
        m_isLock = false;
    }

    void lock() {
        if (!m_isLock) {
            m_mutex.wrlock();
            m_isLock = true;
        }
    }
    void unlock() {
        if (m_isLock) {
            m_mutex.unlock();
            m_isLock = false;
        }
    }

private:
    T &m_mutex;
    bool m_isLock;
};

// 互斥锁
class Mutex : Noncopy {
public:
    Mutex() {
        pthread_mutex_init(&m_mutex, nullptr);
    }
    ~Mutex() {
        pthread_mutex_destroy(&m_mutex);
    }

    void lock() {
        pthread_mutex_lock(&m_mutex);
    }
    void unlock() {
        pthread_mutex_unlock(&m_mutex);
    }

private:
    pthread_mutex_t m_mutex;
};

// 读写锁
class RWMutex : public Noncopy {
public:
    RWMutex() {
        pthread_rwlock_init(&m_lock, nullptr);
    }
    ~RWMutex() {
        pthread_rwlock_destroy(&m_lock);
    }

    void rdlock() {
        pthread_rwlock_rdlock(&m_lock);
    }
    void wrlock() {
        pthread_rwlock_wrlock(&m_lock);
    }
    void unlock() {
        pthread_rwlock_unlock(&m_lock);
    }

private:
    pthread_rwlock_t m_lock;
};

} // namespace jaysheng

#endif // !__JAYSHENG_MUTEX_H__