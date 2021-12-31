/*============================================================================
Copyright 2017-2021 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include "g4environment.h"
#include "G4Version.hh"

// --------------------------------------------------------------------------
namespace {

constexpr int kNumEnvironmentVars = 11;

std::string kG4ENV_LIST [] =
{
  "G4ABLADATA",
  "G4LEDATA",
  "G4ENSDFSTATEDATA",
  "G4INCLDATA",
  "G4NEUTRONHPDATA",
  "G4PARTICLEXSDATA",
  "G4PIIDATA",
  "G4SAIDXSDATA",
  "G4LEVELGAMMADATA",
  "G4RADIOACTIVEDATA",
  "G4REALSURFACEDATA"
};

std::string kG4ENV_PREFIX_LSIT [] =
{
  "G4ABLA",
  "G4EMLOW",
  "G4ENSDFSTATE",
  "G4INCL",
  "G4NDL",
  "G4PARTICLEXS",
  "G4PII",
  "G4SAIDDATA",
  "PhotonEvaporation",
  "RadioactiveDecay",
  "RealSurface"
};

std::map<int, std::string> kVer_G4ABLADATA =
{
  {1020, "3.0"}, {1021, "3.0"}, {1022, "3.0"}, {1023, "3.0"},
  {1030, "3.0"}, {1031, "3.0"}, {1032, "3.0"}, {1033, "3.0"},
  {1040, "3.1"}, {1041, "3.1"}, {1042, "3.1"}, {1043, "3.1"},
  {1050, "3.1"}, {1051, "3.1"},
  {1060, "3.1"}, {1061, "3.1"}, {1062, "3.1"}, {1063, "3.1"},
  {1070, "3.1"}, {1071, "3.1"}, {1072, "3.1"}, {1073, "3.1"},
  {1100, "3.1"}
};

std::map<int, std::string> kVer_G4LEDATA =
{
  {1020, "6.48"}, {1021, "6.48"},  {1022, "6.48"},  {1023, "6.48"},
  {1030, "6.50"}, {1031, "6.50"},  {1032, "6.50"},  {1033, "6.50"},
  {1040, "7.3"},  {1041, "7.3"},   {1042, "7.3"},   {1043, "7.3"},
  {1050, "7.7"},  {1051, "7.7"},
  {1060, "7.9"},  {1061, "7.9.1"}, {1062, "7.9.1"}, {1063, "7.9.1"},
  {1070, "7.13"}, {1071, "7.13"},  {1072, "7.13"},  {1073, "7.13"},
  {1100, "8.0"}
};

std::map<int, std::string> kVer_G4ENSDFSTATEDATA=
{
  {1020, "1.2"}, {1021, "1.2.1"}, {1022, "1.2.3"}, {1023, "1.2.3"},
  {1030, "2.1"}, {1031, "2.1"},   {1032, "2.1"},   {1033, "2.1"},
  {1040, "2.2"}, {1041, "2.2"},   {1042, "2.2"},   {1043, "2.2"},
  {1050, "2.2"}, {1051, "2.2"},
  {1060, "2.2"}, {1061, "2.2"},   {1062, "2.2"},   {1063, "2.2"},
  {1070, "2.3"}, {1071, "2.3"},   {1072, "2.3"},   {1073, "2.3"},
  {1100, "2.3"}
};

std::map<int, std::string> kVer_G4INCLDATA =
{
  {1020, "0.0"}, {1021, "0.0"}, {1022, "0.0"}, {1023, "0.0"},
  {1030, "0.0"}, {1031, "0.0"}, {1032, "0.0"}, {1033, "0.0"},
  {1040, "0.0"}, {1041, "0.0"}, {1042, "0.0"}, {1043, "0.0"},
  {1050, "1.0"}, {1051, "1.0"},
  {1060, "1.0"}, {1061, "1.0"}, {1062, "1.0"}, {1063, "1.0"},
  {1070, "1.0"}, {1071, "1.0"}, {1072, "1.0"}, {1073, "1.0"},
  {1100, "1.0"}
};

std::map<int, std::string>  kVer_G4NEUTRONHPDATA=
{
  {1020, "4.5"}, {1021, "4.5"}, {1022, "4.5"}, {1023, "4.5"},
  {1030, "4.5"}, {1031, "4.5"}, {1032, "4.5"}, {1033, "4.5"},
  {1040, "4.5"}, {1041, "4.5"}, {1042, "4.5"}, {1043, "4.5"},
  {1050, "4.5"}, {1051, "4.5"},
  {1060, "4.6"}, {1061, "4.6"}, {1062, "4.6"}, {1063, "4.6"},
  {1070, "4.6"}, {1071, "4.6"}, {1072, "4.6"}, {1073, "4.6"},
  {1100, "4.6"}
};

std::map<int, std::string>  kVer_G4PARTICLEXSDATA=
{
  {1020, "0.0"}, {1021, "0.0"},   {1022, "0.0"}, {1023, "0.0"},
  {1030, "0.0"}, {1031, "0.0"},   {1032, "0.0"}, {1033, "0.0"},
  {1040, "0.0"}, {1041, "0.0"},   {1042, "0.0"}, {1043, "0.0"},
  {1050, "1.1"}, {1051, "1.1"},
  {1060, "2.1"}, {1061, "2.1"},   {1062, "2.1"}, {1063, "2.1"},
  {1070, "3.1"}, {1071, "3.1.1"}, {1072, "3.1.1"}, {1073, "3.1.1"},
  {1100, "4.0"}
};

std::map<int, std::string>  kVer_G4PIIDATA=
{
  {1020, "1.3"}, {1021, "1.3"}, {1022, "1.3"}, {1023, "1.3"},
  {1030, "1.3"}, {1031, "1.3"}, {1032, "1.3"}, {1033, "1.3"},
  {1040, "1.3"}, {1041, "1.3"}, {1042, "1.3"}, {1043, "1.3"},
  {1050, "1.3"}, {1051, "1.3"},
  {1060, "1.3"}, {1061, "1.3"}, {1062, "1.3"}, {1063, "1.3"},
  {1070, "1.3"}, {1071, "1.3"}, {1072, "1.3"}, {1073, "1.3"},
  {1100, "1.3"}
};

std::map<int, std::string>  kVer_G4SAIDXSDATA=
{
  {1020, "1.1"}, {1021, "1.1"}, {1022, "1.1"}, {1023, "1.1"},
  {1030, "1.1"}, {1031, "1.1"}, {1032, "1.1"}, {1033, "1.1"},
  {1040, "1.1"}, {1041, "1.1"}, {1042, "1.1"}, {1043, "1.1"},
  {1050, "2.0"}, {1051, "2.0"},
  {1060, "2.0"}, {1061, "2.0"}, {1062, "2.0"}, {1063, "2.0"},
  {1070, "2.0"}, {1071, "2.0"}, {1072, "2.0"}, {1073, "2.0"},
  {1100, "2.0"}
};

std::map<int, std::string>  kVer_G4LEVELGAMMADATA=
{
  {1020, "3.2"}, {1021, "3.2"},   {1022, "3.2"},   {1023, "3.2"},
  {1030, "4.3"}, {1031, "4.3.2"}, {1032, "4.3.2"}, {1033, "4.3.2"},
  {1040, "5.2"}, {1041, "5.2"},   {1042, "5.2"},   {1043, "5.2"},
  {1050, "5.3"}, {1051, "5.3"},
  {1060, "5.5"}, {1061, "5.5"},   {1062, "5.5"},   {1063, "5.5"},
  {1070, "5.7"}, {1071, "5.7"},   {1072, "5.7"},   {1073, "5.7"},
  {1100, "5.7"}
};

std::map<int, std::string>  kVer_G4RADIOACTIVEDATA=
{
  {1020, "4.3"}, {1021, "4.3.1"}, {1022, "4.3.2"}, {1023, "4.3.2"},
  {1030, "5.1"}, {1031, "5.1.1"}, {1032, "5.1.1"}, {1033, "5.1.1"},
  {1040, "5.2"}, {1041, "5.2"},   {1042, "5.2"},   {1043, "5.2"},
  {1050, "5.3"}, {1051, "5.3"},
  {1060, "5.4"}, {1061, "5.4"},   {1062, "5.4"},   {1063, "5.4"},
  {1070, "5.6"}, {1071, "5.6"},   {1072, "5.6"},   {1073, "5.6"},
  {1100, "5.6"}
};

std::map<int, std::string>  kVer_G4REALSURFACEDATA=
{
  {1020, "1.0"},   {1021, "1.0"},   {1022, "1.0"},   {1023, "1.0"},
  {1030, "1.0"},   {1031, "1.0"},   {1032, "1.0"},   {1033, "1.0"},
  {1040, "2.1"},   {1041, "2.1"},   {1042, "2.1.1"}, {1043, "2.1.1"},
  {1050, "2.1.1"}, {1051, "2.1.1"},
  {1060, "2.1.1"}, {1061, "2.1.1"}, {1062, "2.1.1"}, {1063, "2.1.1"},
  {1070, "2.2"},   {1071, "2.2"},   {1072, "2.2"},   {1073, "2.2"},
  {1100, "2.2"}
};

std::map<int, std::string> kVer_LIST [] =
{
  kVer_G4ABLADATA,
  kVer_G4LEDATA,
  kVer_G4ENSDFSTATEDATA,
  kVer_G4INCLDATA,
  kVer_G4NEUTRONHPDATA,
  kVer_G4PARTICLEXSDATA,
  kVer_G4PIIDATA,
  kVer_G4SAIDXSDATA,
  kVer_G4LEVELGAMMADATA,
  kVer_G4RADIOACTIVEDATA,
  kVer_G4REALSURFACEDATA
};

} // end of namespace

// ==========================================================================
std::string G4Environment::data_dir_ {"none"};

// --------------------------------------------------------------------------
void G4Environment::SetDataDir(const std::string& dir)
{
  data_dir_ = dir;
}

// --------------------------------------------------------------------------
void G4Environment::CheckEnvironment()
{
  for (auto env : ::kG4ENV_LIST ) {
    auto envstr = std::getenv(env.c_str());
    if ( envstr == nullptr ) {
      std::cout << "[ Error ] Env. Var. (" << env
                << ") is not set." << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
}

// --------------------------------------------------------------------------
void G4Environment::SetEnvironment()
{
  int g4ver = G4VERSION_NUMBER;

  // set env vars
  for ( auto i = 0; i < ::kNumEnvironmentVars; i++ ) {
    auto envvar = ::kG4ENV_LIST[i];
    auto envstr = std::getenv(envvar.c_str());
    if ( envstr == nullptr ) {
      if ( data_dir_ == "none" ) {
        std::cout << "[ Error ] G4Data directory is not set." << std::endl;
        std::exit(EXIT_FAILURE);
      }

      auto var = data_dir_ + "/" + ::kG4ENV_PREFIX_LSIT[i] +
                                   ::kVer_LIST[i][g4ver];
      setenv(::kG4ENV_LIST[i].c_str(), var.c_str(), 1);

    } else {
      std::string user_var(envstr);
      std::string valid_var = ::kG4ENV_PREFIX_LSIT[i] + ::kVer_LIST[i][g4ver];
      if ( user_var.find(valid_var) == std::string::npos ) {
        std::cout << "[ Error ] Invalid data environment variable for "
                  << ::kG4ENV_LIST[i] << std::endl;
        std::cout << " * " << user_var << " is set against expected "
                  << valid_var << std::endl;
        std::exit(EXIT_FAILURE);
      }
    }
  }
}

// --------------------------------------------------------------------------
void G4Environment::PrintEnvironment()
{
  for ( auto env : ::kG4ENV_LIST ) {
    auto var = std::getenv(env.c_str());
    std::cout << env << "=" << var << std::endl;
  }
}
