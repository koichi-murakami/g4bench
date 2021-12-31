/*============================================================================
Copyright 2017-2021 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef SIM_DATA_H_
#define SIM_DATA_H_

class SimData {
public:
  SimData() = default;
  ~SimData() = default;

  SimData(const SimData&) = delete;
  void operator=(const SimData&) = delete;

  void Initialize();

  void AddStepCount();
  long GetStepCount() const;

  void AddEdep(double val);
  double GetEdep() const;

private:
  long step_count_;
  double edep_;

};

// ==========================================================================
inline void SimData::AddStepCount()
{
  step_count_++;
}

inline long SimData::GetStepCount() const
{
  return step_count_;
}

inline void SimData::AddEdep(double val)
{
  edep_ += val;
}

inline double SimData::GetEdep() const
{
  return edep_;
}

inline void SimData::Initialize()
{
  step_count_ = 0;
  edep_ = 0.;
}

#endif
