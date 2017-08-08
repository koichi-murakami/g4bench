/*============================================================================
  Copyright 2017 Koichi Murakami

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include <sys/times.h>
#include <time.h>
#include <unistd.h>

namespace mpexs {

class Stopwatch {
public:
  Stopwatch();
  ~Stopwatch();

  void Reset();
  void Split();

  double GetRealElapsed() const;
  double GetSystemElapsed() const;
  double GetUserElapsed() const;

  const char* GetClockTime() const;

private:
  clock_t start_clock_, end_clock_;
  tms start_time_, end_time_;

};

} // namespace mpexs

#endif
