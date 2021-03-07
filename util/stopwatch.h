/*============================================================================
  Copyright 2017-2021 Koichi Murakami

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file License for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include <chrono>
#include <ctime>
#include <string>

#ifdef _MSC_VER
#include <time.h>
#define _SC_CLK_TCK  1

extern "C" {
  int sysconf(int);
};

struct tms {
  clock_t tms_utime;    // user time
  clock_t tms_stime;    // system time
  clock_t tms_cutime;   // user time, children
  clock_t tms_cstime;   // system time, children
};

extern "C" {
  extern clock_t times(struct tms*);
};

#else
#include <unistd.h>
#include <sys/times.h>
#endif

namespace kut {

 using g_clock = std::chrono::system_clock;

class Stopwatch {
public:
  Stopwatch();
  ~Stopwatch() = default;

  void Reset();
  void Split();

  double GetRealElapsed() const;
  double GetSystemElapsed() const;
  double GetUserElapsed() const;

  std::string GetClockTime() const;

private:
  g_clock::time_point start_clock_, end_clock_;
  tms start_time_, end_time_;

};

} // end of namespace

#endif
