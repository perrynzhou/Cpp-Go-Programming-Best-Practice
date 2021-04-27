#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>
std::atomic<int> Guard(0);
int PayLoad = 0;
void Read() {
    for(;;) {
        int g= Guard.load(std::memory_order_acquire);
        if(g!=0) {
            std::cout<<" g="<<g<<",payload="<<PayLoad<<std::endl;
        }
    }
}
void Write() {
    std::atomic<int> counter;
    while(true) {
        PayLoad++;
        Guard.store(1,std::memory_order_release);
    }
}
int main() {
    std::thread t1(Read);
    std::thread t2(Write);
    t1.join();
    t2.join();
    return 0;
}