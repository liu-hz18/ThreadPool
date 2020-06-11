#pragma once

#include <pthread.h>
#include <cstdlib>
#include <cstring>
#include <assert.h>
#include <atomic>
#include "Runnable.h"

//Thread can't be shared by multi-threads
class Thread: public Runnable{
    pthread_t m_thread;
    int m_state;
    void* m_args;
    std::atomic_bool is_running;
    int thread_id;
    Runnable* runnable;

    static int thread_tot;
    static void *func(void *args);

public:
    Thread(): m_state(0), m_args(nullptr){
        is_running = false;
        thread_id = thread_tot++;
        runnable = nullptr;
    }
    Thread(Runnable* r): m_state(0), m_args(nullptr){
        is_running = false;
        thread_id = thread_tot++;
        runnable = r;
    }
    virtual ~Thread(){
        m_state = 0;
        m_args = nullptr;
    }
    virtual void run() {
        if(runnable){
            runnable->run();
        }
    }
    void start() {
        m_state = pthread_create(&m_thread, nullptr, Thread::func, this);
        is_running = true;
    }
    void join(){
        if(is_running){
            pthread_join(m_thread, nullptr);
            is_running = false;
        }
    }
    int threadid() const {
        return thread_id;
    }

};

int Thread::thread_tot = 0;

void* Thread::func(void* args){
    Thread* p = static_cast<Thread*>(args);
    p->run();
    return nullptr;
}
