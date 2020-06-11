
#include <time.h>
#include <iostream>
#include "Thread.h"
#include "Runnable.h"
#include "ThreadPool.h"
#include "orderedList.h"

const double readPercent = 0.80;
const int findbound = int(100 * readPercent);
const int insertbound = int((1.0 - readPercent) * 50) + findbound + 1;
orderedList<int> mylist;

class listTest: public Runnable{
    int op, value;

public:
    listTest() = default;
    listTest(int _op, int _val): op(_op), value(_val){}
    void run(){
        if (op < findbound) {//find for 80%
            if(mylist.find(value)){
                printf("threadid %u, find value %d success\n", (unsigned int)pthread_self(), value);
            } else {
                printf("threadid %u, find value %d failed\n", (unsigned int)pthread_self(), value);
            }
        } else if (findbound <= op && op < insertbound) {//delete for 10%
            if(mylist.pop(value)){
                printf("threadid %u, delete value %d success\n", (unsigned int)pthread_self(), value);
            } else {
                printf("threadid %u, delete value %d failed\n", (unsigned int)pthread_self(), value);
            }
        } else {//insert for 10%
            mylist.push(value);
            printf("threadid %u, inserted value %d\n", (unsigned int)pthread_self(), value);
        }
#       ifdef DEBUG
        std::cout << mylist.toString();
#       endif
    }
};


int main(int argc, char const *argv[]){
    if (argc != 3) {
        printf("usage: %s [thread_num] [task_num]\n", argv[0]);
        exit(1);
    }
#   ifdef DEBUG
    printf("Percent: find: %f, insert: %f, delete: %f\n", readPercent, (1.0 - readPercent)/2, (1.0 - readPercent)/2);
#   endif
    int thread_num = atoi(argv[1]), task_num = atoi(argv[2]);
    srand((unsigned)time(0));
    {
        ThreadPool pool(thread_num);
        for(size_t i = 0; i < task_num; i++){
            pool.addTask( new listTest( rand() % 100, rand() % 500 ) );
        }
        pool.join();
    }
    {
        listTest* list_ptr = new listTest( rand() % 100, rand() % 500);
        Thread* athread = new Thread(list_ptr);
        athread->run();
        printf("threadid: %d\n", athread->threadid());
        athread->join();
        delete list_ptr;
        delete athread;
    }
    return 0;
}
