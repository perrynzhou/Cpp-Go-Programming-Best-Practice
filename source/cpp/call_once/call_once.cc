
#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
#include <vector>
#include <condition_variable>
class CallOnce
{
public:
    CallOnce(int thread_count) : thread_count_(thread_count)
    {
        threads_.clear();
    }

    ~CallOnce()
    {
        for (int i = 0; i < thread_count_; i++)
        {
            threads_[i]->join();
        }
        threads_.clear();
    }

    void Do()
    {
        pthread_t self = pthread_self();
        //  std::cout << "current thread:" << self << std::endl;
        // std::unique_lock<std::mutex> lock(mutex_);
        // 简易版本
        //  std::call_once(flag_, []() { std::cout << pthread_self() << ":CallOnce Just Do Once" << std::endl; });

        // 可以传入this,在lamda中获取this中的变量或者函数
        std::call_once(flag_, [this]() { std::cout << pthread_self() << ":CallOnce Just Do Once,ready=" << thread_count_ << std::endl; });
        while (true)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            std::cout << "worker thread:" << self << " wating" << std::endl;
            cv_.wait(lock, [this] { return ready_; });

            // cv_.wait(lock,ready_);
            std::cout << "worker thread:" << self << " working" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            lock.unlock();
            cv_.notify_one();
        }
    }

    void Run()
    {
        pthread_t self = pthread_self();
        for (int i = 0; i < thread_count_; i++)
        {
            threads_.push_back(new std::thread(&CallOnce::Do, this));
        }
        std::cout << " main thread create " << thread_count_ << std::endl;
        int count = 0;
        while (true)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "main thread:" << self << " data " << count << " finish" << std::endl;
            ready_ = true;
            cv_.notify_all();
            count++;
        }
    }

private:
    int thread_count_;
    std::once_flag flag_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::vector<std::thread *> threads_;
    bool ready_ = false;
    bool processed_ = false;
};

int main()
{
    CallOnce once(4);
    std::thread main(&CallOnce::Run, &once);
    main.join();
    return 0;
}