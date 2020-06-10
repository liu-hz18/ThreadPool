#pragma once

#include <pthread.h>

class Mutex;

class CondVar{
    pthread_cond_t m_cond;
public:
    CondVar(){ pthread_cond_init(&m_cond, nullptr); }
    virtual ~CondVar(){ pthread_cond_destroy(&m_cond); }
    CondVar(const CondVar&) = delete;
    CondVar& operator=(const CondVar&) = delete;
    int wait(Mutex *mu){ return pthread_cond_wait(&m_cond, &(mu->m_mutex)); }
    void notify_one(){ pthread_cond_signal(&m_cond); }
    void notify_all(){ pthread_cond_broadcast(&m_cond); }
};
