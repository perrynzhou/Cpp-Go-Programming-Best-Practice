#include <mutex>
#include <condition_variable>
#include <queue>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <thread>
std::mutex mut_;
std::condition_variable cond_;
std::queue<int> data_;

void prepare_thread()
{
    while (true)
    {
        int value = rand() % 1024;
        {
            std::lock_guard<std::mutex> lk(mut_);
            data_.push(value);
        }

        cond_.notify_one();
        std::cout << "prepare val=" << value << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
void consumed_thread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(mut_);
        cond_.wait(lk, [] { return !data_.empty(); });
        int val = data_.front();
        data_.pop();
        lk.unlock();
        std::cout << "consumed val=" << val << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
int main()
{
    std::thread t1(prepare_thread);
    std::thread t2(consumed_thread);
    t1.join();
    t2.join();
    return 0;
}