/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef SIM_DATA_H_
#define SIM_DATA_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

class SimData {
public:
  SimData();
  virtual ~SimData();

  void Initialize();

  void AddStepCount();
  long GetStepCount() const;

  void AddEdep(double val);
  double GetEdep() const;

private:
  DISALLOW_COPY_AND_ASSIGN(SimData);

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

#endif
