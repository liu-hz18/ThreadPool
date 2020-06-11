
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <future>
#include <stdexcept>
#include <mutex>
#include "threadpool.h"

std::mutex print_mutex;

// function 1
void Func1 (int id) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> locker(print_mutex);
        std::cout << "called Func1(" << id << ") in thread " << std::this_thread::get_id() << std::endl;
    }
}

// function 2
class Func2 {
public:
    int operator() (int a, int b) {
        int result = a + b;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            std::lock_guard<std::mutex> locker(print_mutex);
            std::cout << "called Func2(" << a << ", " << b << ") = " << result << " in thread " << std::this_thread::get_id() << std::endl;
        }
        return result;
    }
};

int main(int argc, char const *argv[]){
    std::default_random_engine re;
    std::uniform_int_distribution<int> u(0, 500);

    try {
        threadPool tp1(4);
        for(size_t i = 0; i < 50; i++) {
            tp1.commit(Func1, u(re));
        }
        tp1.join();
    } catch (std::exception e) {
        std::cout << "main thread:" << e.what() << std::endl;
    }

    try {
        threadPool tp2(6);
        std::vector< std::future<int> > func2_rets;　//接收返回值
        for(size_t i = 0; i < 50; i++) {
            func2_rets.emplace_back( tp2.commit(Func2(), i, i+1) );
        }
        tp2.join();
        // print results
        for(auto&& rets: func2_rets) {
            std::cout << rets.get() << " ";
        }
        std::cout << std::endl;
    } catch (std::exception e) {
        std::cout << "main thread:" << e.what() << std::endl;
    }

    return 0;
}