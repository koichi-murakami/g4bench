/*============================================================================
Copyright 2017-2019 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef APP_BUILDER_H_
#define APP_BUILDER_H_

#include <string>
#include "G4VUserActionInitialization.hh"

class SimData;

class AppBuilder : public G4VUserActionInitialization {
public:
  AppBuilder();
  ~AppBuilder();

  void BuildApplication();

  void SetTestingFlag(bool val);
  void SetTestingFlag(bool val, const std::string& bname,
                                const std::string& cname);
  virtual void Build() const;
  virtual void BuildForMaster() const;

private:
  SimData* simdata_;
  int nvec_;
  bool qtest_;
  std::string bench_name_;
  std::string cpu_name_;
};

// ==========================================================================
inline void AppBuilder::SetTestingFlag(bool val)
{
  qtest_ = val;
}

inline void AppBuilder::SetTestingFlag(bool val, const std::string& bname,
                                                 const std::string& cname)
{
  qtest_ = val;
  bench_name_ = bname;
  cpu_name_ = cname;
}

#endif
