#pragma once

#include <pthread.h>
#include "Cond.h"

class Mutex{
    friend class CondVar;
    pthread_mutex_t m_mutex;
public:
    Mutex(){ pthread_mutex_init(&m_mutex, nullptr); }
    virtual ~Mutex(){ pthread_mutex_destroy(&m_mutex); }
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;
    void lock(){ pthread_mutex_lock(&m_mutex); }
    void unlock(){ pthread_mutex_unlock(&m_mutex); }
};

class ReadWriteMutex{
    friend class CondVar;
    pthread_rwlock_t m_rwlock;
public:
    ReadWriteMutex(){ pthread_rwlock_init(&m_rwlock, nullptr); }
    virtual ~ReadWriteMutex(){ pthread_rwlock_destroy(&m_rwlock); }
    ReadWriteMutex(const ReadWriteMutex&) = delete;
    ReadWriteMutex& operator=(const ReadWriteMutex&) = delete;
    void readlock(){ pthread_rwlock_rdlock(&m_rwlock); }
    void writelock(){ pthread_rwlock_wrlock(&m_rwlock); }
    void unlock(){ pthread_rwlock_unlock(&m_rwlock); }
};