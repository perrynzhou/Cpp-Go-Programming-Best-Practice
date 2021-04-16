
#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
#include <vector>
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
        std::cout << "current thread:" <<pthread_self()<< std::endl;
        std::call_once(flag_, []() { std::cout <<pthread_self()<< ":CallOnce Just Do Once"<<std::endl; });
    }
    
    void Run()
    {
        for (int i = 0; i < thread_count_; i++)
        {
            threads_.push_back(new std::thread(&CallOnce::Do, this));
        }
    }

private:
    int thread_count_;
    std::once_flag flag_;
    std::vector<std::thread *> threads_;
};

int main()
{
    CallOnce once(4);
    once.Run();
    return 0;
}