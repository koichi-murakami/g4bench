/*============================================================================
  Copyright 2017 Koichi Murakami

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef TIME_HISTORY_H_
#define TIME_HISTORY_H_

#include <map>
#include <string>
#include "util/stopwatch.h"

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete
#endif

class TimeHistory {
public:
  static TimeHistory* GetTimeHistory();
  virtual ~TimeHistory() = default;

  DISALLOW_COPY_AND_ASSIGN(TimeHistory);

  void TakeSplit(const std::string& key);

  double TakeSplit();

  bool FindAKey(const std::string& key) const;

  double GetTime(const std::string& key) const;

  void ShowHistory(const std::string& key) const;

  void ShowAllHistories() const;

  void ShowClock(const std::string& prefix="") const;

private:
  TimeHistory();

  Stopwatch sw_;
  double t0_;
  std::map<std::string, double> split_history_;

};

#endif
