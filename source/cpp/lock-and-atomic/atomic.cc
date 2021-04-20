
#include <mutex>
#include <cstdint>
#include <atomic>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

class LockCounter
{
public:
    LockCounter(int thread_count, uint64_t count_times) : thread_count_(thread_count), count_times_(count_times), count_(0), duration_(0) {}
    ~LockCounter() {}

    void Run()
    {
        auto start = std::chrono::system_clock::now();

        for (int i = 0; i < thread_count_; i++)
        {
            std::thread t1([this] { this->Incr(); });
            threads_.push_back(std::move(t1));
        }
        if (!threads_.empty())
        {

            for (auto &thd : threads_)
            {
                thd.join();
            }
            auto end = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            duration_ = elapsed.count();
            std::cout << "LockCounter Counter=" << count_ << " Consumed:" << duration_ << " microsecond" << std::endl;
            thread_count_ = 0;
            threads_.clear();
        }
    }
    unsigned long duration_;

private:
    void Incr()
    {
        for (uint64_t i = 0; i < count_times_; i++)
        {
            std::lock_guard<std::mutex> lk(mut_);
            count_++;
        }
    };
    std::mutex mut_;
    uint64_t count_;
    int thread_count_;
    uint64_t count_times_;
    std::vector<std::thread> threads_;
};
class AtomicCounter
{
public:
    AtomicCounter(int thread_count, uint64_t count_times) : thread_count_(thread_count), count_times_(count_times), count_(0), duration_(0) {}
    ~AtomicCounter() {}
    void Run()
    {
        auto start = std::chrono::system_clock::now();

        for (int i = 0; i < thread_count_; i++)
        {
            std::thread t1([this] { this->Incr(); });
            threads_.push_back(std::move(t1));
        }
        if (!threads_.empty())
        {
            for (auto &thd : threads_)
            {
                thd.join();
            }
            auto end = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            duration_ = elapsed.count();
            std::cout << "AtomicCounter Counter=" << count_ << " Consumed:" << duration_ << " microsecond" << std::endl;
            thread_count_ = 0;
            threads_.clear();
        }
    }
    unsigned long duration_;

private:
    void Incr()
    {
        for (uint64_t i = 0; i < count_times_; i++)
        {
            count_++;
        }
    };
    std::atomic<uint64_t> count_;
    int thread_count_;
    std::vector<std::thread> threads_;
    uint64_t count_times_;
};
int main()
{

    LockCounter lc(3, 1999999);
    lc.Run();
    AtomicCounter ac(3, 1999999);
    ac.Run();
    double lock_time = static_cast<double>(lc.duration_);
    double atomic_time = static_cast<double>(lc.duration_);

    std::cout << "LockCounter/AtomicCounter  Microsecond  =" << static_cast<double>(lock_time / atomic_time) << std::endl;
    ac.Run();
    return 0;
}