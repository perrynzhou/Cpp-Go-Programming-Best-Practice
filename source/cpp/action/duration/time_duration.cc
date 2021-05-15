/*************************************************************************
  > File Name: time_duration.cc
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 六  5/15 17:08:44 2021
 ************************************************************************/
#include "time_duration.h"
namespace timer
{
  TimeDuration::TimeDuration(std::string tag, TimeType type)
  {
    tag_ = tag;
    type_ = type;
    begin_ = std::chrono::high_resolution_clock::now();
  }

  long long TimeDuration::Elapsed()
  {
    long long time = 0;
    switch (type_)
    {
    case TimeType::Microsecond:
      time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - begin_).count();
      if (flag_)
      {
        std::cout << "time { " << tag_ << " } " << static_cast<double>(time) << " ms " << std::endl;
      }
      break;
    case TimeType::Second:
      time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - begin_).count();
      if (flag_)
      {
        std::cout << "time { " << tag_ << " } " << static_cast<double>(time) << " s " << std::endl;
      }
      break;
    }
    
    flag_ = false;
    return time;
  }
  TimeDuration::~TimeDuration()
  {
    if (flag_)
    {
      flag_ = false;
      long long time = Elapsed();
      if (type_ == TimeType::Microsecond)
      {
        std::cout << "time { " << tag_ << " } " << static_cast<double>(time) << " ms " << std::endl;
        return;
      }
      std::cout << "time { " << tag_ << " } " << static_cast<double>(time) << " s " << std::endl;
    }
  }
  void TimeDuration::Reset(std::string tag, TimeType type)
  {
    begin_ = std::chrono::high_resolution_clock::now();
    type_ = type;
    tag_ = tag;
    flag_ = true;
  }
} // timer namesapce