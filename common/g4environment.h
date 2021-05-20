/*============================================================================
Copyright 2017-2021 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef G4ENVIRONMENT_H_
#define G4ENVIRONMENT_H_

#include<string>

class G4Environment {
public:
  G4Environment() = default;
  ~G4Environment() = default;

  static void SetDataDir(const std::string& dir);
  static void CheckEnvironment();
  static void SetEnvironment();
  static void PrintEnvironment();

private:
  static std::string data_dir_;

};

#endif