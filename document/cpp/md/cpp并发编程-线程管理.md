###  并发编程-管理线程

#### 线程管理
- c++中使用标准库的thread来实现线程的相关功能，一旦开始了线程，要么等待完成(thread.join()),要么让它自行运行(thread.detach()).线程的在销毁前会调用线程的析构函数，这个析构函数会调用std::terminate().
```
#include <thread>
#include <iostream>
void doSomeThing()
{
    std::cout << "call doSomeThing" << std::endl;
}

int main(void)
{

    std::thread td1([] {
        doSomeThing();
    });
    td1.join();
    return 0;
}
```
- 在应用开发中如果想要等待线程，就需要仔细的选择在代码的哪个位置调用join(),如果线程在开始之后但是又在调用join()之前发生了异常，对join()调用就容易被跳过。如果想确保线程在必须在函数退出前完成，可以使用RAII的语法。构造时候初始化线程，析构时候join线程
```
#include <thread>
class thread_guard
{
    std::thread &t_;

public:
    explicit thread_guard(std::thread &t) : t_(t) {}
    ~thread_guard()
    {
        if (t_.joinable())
        {
            t_.join();
        }
    }
    thread_guard(thread_guard const &) = delete;
    thread_guard &operator=(thread_guard const &) = delete;
};
```