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