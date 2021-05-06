/*************************************************************************
  > File Name: fork_demo.cc
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 四  5/ 6 06:50:04 2021
 ************************************************************************/

#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int x = 100;
void wait_all()
{
    int pid;
    int status;
    for (;;)
    {
        pid = wait(&status);
        if (pid == -1)
        {
            if (errno == ECHILD)
            {
                break;
            }
            perror("wait");
            exit(-1);
        }
    }
}
int main()
{
    int pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(-1);
    }
    if (pid == 0)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        x = 200;
        std::cout << "return child  process:" << getpid() << ",x=" << x << std::endl;
    }
    else
    {
        std::cout << "return parent process:" << getpid() << ",x=" << x << std::endl;
    }
    wait_all();
    return 0;
}
