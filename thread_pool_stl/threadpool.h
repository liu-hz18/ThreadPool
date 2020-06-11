#pragma once
#include <queue>
#include <vector>
#include <thread>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <future>

class threadPool{
    using Task = std::function<void()>;
    std::vector<std::thread> m_pool; //thread Pool
    std::queue<Task> m_tasks; //task queue
    std::mutex m_mutex; //互斥量
    std::condition_variable m_cv; //conditional var
    std::atomic_bool m_finished; //完成提交
    size_t m_thread_num; //线程数量

public:
    explicit threadPool(size_t n): m_thread_num(n), m_finished(false){
        for(size_t i = 0; i < m_thread_num; ++i) {
            m_pool.emplace_back( [this] {
                while(1) {
                    std::function<void()> task;
                    {   
                        //condition_variable cv; 条件变量， 需要配合 unique_lock 使用；unique_lock 相比 lock_guard 的好处是：可以随时 unlock() 和 lock()。 cv.wait() 之前需要持有 mutex，wait 本身会 unlock() mutex，如果条件满足则会重新持有 mutex。
                        std::unique_lock<std::mutex> locker(this->m_mutex);
                        this->m_cv.wait( locker, [this] {
                                return (this->m_finished || (!this->m_tasks.empty()));
                            }
                        );
                        if(this->m_finished && this->m_tasks.empty())
                            return;
                        task = std::move(this->m_tasks.front());
                        this->m_tasks.pop();
                    }
                    task();
                }
            });
        }
    }
    threadPool(const threadPool&) = delete;
    threadPool& operator=(const threadPool&) = delete;
    virtual ~threadPool() = default;
    void join() {
        m_finished = true;
        m_cv.notify_all();
        for(auto& thread: m_pool) {
            if(thread.joinable()) thread.join();
        }
    }
    template <class F, class... Args>　//变长模板
    auto commit(F&& f, Args&&... args) {
        if (m_finished) 
            throw std::runtime_error("commit when threadPool has joined"); 
        using return_type = typename std::result_of<F(Args...)>::type;
        // 将函数f 和 其形参args 绑定
        auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<return_type> res = task->get_future();
        {
            std::lock_guard<std::mutex> locker(m_mutex);
            m_tasks.emplace( [task](){ (*task)(); }); 
        }
        m_cv.notify_one();
        return res;
    }
};