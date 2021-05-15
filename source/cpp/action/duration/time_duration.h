/*************************************************************************
  > File Name: time_log.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 六  5/15 17:09:02 2021
 ************************************************************************/

#ifndef _TIME_DURATION_H
#define _TIME_DURATION_H
#include <chrono>
#include <string>
#include <iostream>
namespace timer{
enum class TimeType: unsigned char {
   Microsecond,
   Second,
};
class TimeDuration
{
public:
   TimeDuration(std::string tag,TimeType type=TimeType::Microsecond);
   ~TimeDuration();
   void Reset(std::string tag,TimeType type=TimeType::Microsecond);
   long long Elapsed();

private:
   std::string tag_;
   TimeType  type_;
   bool flag_;
   std::chrono::time_point<std::chrono::high_resolution_clock> begin_;
};
}
#endif
