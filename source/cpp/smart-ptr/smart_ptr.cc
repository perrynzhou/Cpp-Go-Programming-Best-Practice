
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
class T1;
class T2;
class T3;
class T1
{
public:
    T1()
    {

        std::cout << "T1()" << std::endl;
    }
    ~T1()
    {
        std::cout << "~T1()" << std::endl;
    }
    std::shared_ptr<T2> t2_ptr;
};

class T2
{
public:
    T2()
    {

        std::cout << "T2()" << std::endl;
    }
    ~T2()
    {
        std::cout << "~T2()" << std::endl;
    }
    // 这样会出现循环引用导致无法析构
    // std::shared_ptr<T1> t1_ptr;
    std::weak_ptr<T1> t1_ptr;
};
class T3
{
public:
    T3()
    {

        std::cout << "T3()" << std::endl;
    }
    ~T3()
    {
        std::cout << "~T3()" << std::endl;
    }
};
int main(void)
{

    {
        auto t3 = std::unique_ptr<T3>(new T3);
        // 当前只能采用move来转移unique_ptr
        std::unique_ptr<T3> a1 = std::move(t3);
        // unique的智能指针构造函数被禁用，所以无法赋值和拷贝，只能move，所以下面这个赋值编译器会报错
        std::unique_ptr<T3> a2 = a1;
    }
    std::this_thread::sleep_for(std::chrono::seconds(0));
    return 0;
}