/*============================================================================
  Copyright 2017 Koichi Murakami

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#include "util/stopwatch.h"

// --------------------------------------------------------------------------
Stopwatch::Stopwatch()
{
  Reset();
}

// --------------------------------------------------------------------------
Stopwatch::~Stopwatch()
{
}

// --------------------------------------------------------------------------
void Stopwatch::Reset()
{
  start_clock_ = times(&start_time_);
}

// --------------------------------------------------------------------------
void Stopwatch::Split()
{
  end_clock_ = times(&end_time_);
}

// --------------------------------------------------------------------------
double Stopwatch::GetRealElapsed() const
{
  double diff = end_clock_ - start_clock_;
  return diff / sysconf(_SC_CLK_TCK);
}

// --------------------------------------------------------------------------
double Stopwatch::GetSystemElapsed() const
{
  double diff = end_time_.tms_stime - start_time_.tms_stime;
  return diff / sysconf(_SC_CLK_TCK);
}

// --------------------------------------------------------------------------
double Stopwatch::GetUserElapsed() const
{
  double diff = end_time_.tms_utime - start_time_.tms_utime;
  return diff / sysconf(_SC_CLK_TCK);
}

// --------------------------------------------------------------------------
const char* Stopwatch::GetClockTime() const
{
   time_t timer;
   time(&timer);

   return ctime(&timer);
}
