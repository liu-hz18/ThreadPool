#pragma once

#include "Mutex.h"

class MutexLocker{
    Mutex* m_mutex;
public:
    MutexLocker(Mutex *mu): m_mutex(mu){ m_mutex->lock(); }
    virtual ~MutexLocker() {m_mutex->unlock(); }
    MutexLocker(const MutexLocker &) = delete;
    MutexLocker& operator=(const MutexLocker &) = delete;
};


class ReadLocker{
    ReadWriteMutex* m_rmutex;
public:
    ReadLocker(ReadWriteMutex *mu): m_rmutex(mu){ m_rmutex->readlock(); }
    virtual ~ReadLocker() { m_rmutex->unlock(); }
    ReadLocker(const ReadLocker &) = delete;
    ReadLocker& operator=(const ReadLocker &) = delete;
};


class WriteLocker{
    ReadWriteMutex* m_wmutex;
public:
    WriteLocker(ReadWriteMutex *mu): m_wmutex(mu){ m_wmutex->writelock(); }
    virtual ~WriteLocker() { m_wmutex->unlock(); }
    WriteLocker(const WriteLocker &) = delete;
    WriteLocker& operator=(const WriteLocker &) = delete;
};
