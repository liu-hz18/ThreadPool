
#pragma once

//Runable can be shared by multi-threads
class Runnable{
    static int taskNumber;
    int taskId;

public:
    Runnable(){
        taskId = taskNumber++;
    }
    virtual void run() = 0;
};

int Runnable::taskNumber = 0;
