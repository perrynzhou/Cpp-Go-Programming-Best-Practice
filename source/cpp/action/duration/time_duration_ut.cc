/*************************************************************************
  > File Name: timme_duration_ut.cc
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 六  5/15 17:27:09 2021
 ************************************************************************/

#include "time_duration.h"
#include <thread>
#include <iostream>
int main()
{
  auto func = []() {
    for (int i = 0; i < 1; i++)
    {
      std::cout << "i =" << i << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  };
  {
    timer::TimeDuration du("func", timer::TimeType::Second);
    func();
    du.Elapsed();
    du.Reset("func2");
    func();
    du.Elapsed();
    du.Reset("func3", timer::TimeType::Second);
    func();
    du.Elapsed();
    du.Reset("func4", timer::TimeType::Second);
    func();
  }
}
