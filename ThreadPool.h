#pragma once

#include <cstdio>
#include <list>
#include <atomic>

#include "Mutex.h"
#include "Locker.h"
#include "Cond.h"
#include "Runnable.h"

using std::list;

class ThreadPool{
    unsigned thread_num;
    std::atomic_bool finished;
    pthread_t* threads;
    list<Runnable*> taskq;
    Mutex mu;
    CondVar cond;

public:
    static void* task(void* args);

    ThreadPool(unsigned _n): thread_num(_n), threads(new pthread_t[thread_num]){
        finished = false;
        for(size_t i = 0; i < thread_num; i++){
            pthread_create(&threads[i], nullptr, ThreadPool::task, this);
        }
    }
    virtual ~ThreadPool(){
        for(auto iter: taskq){ delete iter; }
    }
    void join(){
        finished = true;
        cond.notify_all();
        for(size_t i = 0; i < thread_num; i++){
            pthread_join(threads[i], nullptr);
        }
    }
    void addTask(Runnable* t){
        MutexLocker locker(&mu);
        taskq.push_back(t);
        cond.notify_one();
    }
};

void* ThreadPool::task(void* args){
    ThreadPool *thp = static_cast<ThreadPool*>(args);
    while(1){
        Runnable* t;
        {
            MutexLocker locker(&thp->mu);
            while(thp->taskq.empty() && !thp->finished){
                while( thp->cond.wait(&thp->mu) != 0 )
                    ;
            }
            if(thp->taskq.empty()){
                break;
            }
            t = thp->taskq.front();
            thp->taskq.pop_front();
        }
        t->run();
    }
    return nullptr;
}