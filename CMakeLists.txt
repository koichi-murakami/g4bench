# - Top-level CMakeLists.txt for G4Bench

# cmake 2.8 or later is required for CUDA support
cmake_minimum_required(VERSION 2.8)

#------------------------------------------------------------------------------
project(CUGEANT4)
#------------------------------------------------------------------------------
# installation prefixes for libraries
set(CMAKE_INSTALL_PREFIX $ENV{HOME}/Library/g4bench)
set(GEANT4_INSTALL $ENV{HOME}/Library/geant4)
set(DCMTK_DIR /opt/dcmtk)
set(GTEST_ROOT /opt/gtest)

# skip CUDA
set(SKIP_CUDA FALSE CACHE BOOL "Skip CUDA if not installed")

# CUDA real
set(REAL_AS_DOUBLE FALSE CACHE BOOL "Real as Double")

# debug mode
set(DEBUG FALSE CACHE BOOL "Debug Mode (Debug On)")

#------------------------------------------------------------------------------
# Do not edit below
#------------------------------------------------------------------------------
# cmake modules
set(CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake/Modules
    ${CMAKE_MODULE_PATH})

include(SetInstallPath)

# searching packages...
find_package(Geant4 REQUIRED)
if(NOT SKIP_CUDA)
  find_package(CUDA REQUIRED)
endif()
find_package(Boost 1.36.0)
find_package(GTest REQUIRED)
find_package(DCMTK REQUIRED)
find_package(ROOT)
include(ModifyDCMTK)

#------------------------------------------------------------------------------
# parameters for building
message(STATUS "--------------------------------------------------------")
message(STATUS "Parameters for building")

# build options
if(NOT CMAKE_BUILD_TYPE)
  if(DEBUG)
    set(CMAKE_BUILD_TYPE "Debug")
  else()
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
  endif()
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")

# compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS
      "-W -pedantic -Wall -Wwrite-strings -Wno-variadic-macros -Wno-long-long -pipe -pthread")

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS
      "-W -pedantic -Wall -Wwrite-strings -Wno-variadic-macros -Wno-long-long -pipe")

elseif(CMAKE_CXX_COMPILER MATCHES "icpc.*|icc.*")
  set(CMAKE_CXX_FLAGS
      "-w1 -Wall -fp-model precise -ipo")
  set(CMAKE_EXE_LINKER_FLAGS "-i-dynamic -limf")
endif()


message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "--------------------------------------------------------")

#------------------------------------------------------------------------------
# cug4_configure.h
set(CUG4_VERSION_MAJOR 1)
set(CUG4_VERSION_MINOR 0)
set(CUG4_VERSION_BUILD 0)

configure_file(
  "${PROJECT_SOURCE_DIR}/cug4config.h.in"
  "${PROJECT_BINARY_DIR}/cug4config.h"
)

#------------------------------------------------------------------------------
# add subdiretories...
include(CTest)
enable_testing()

# adding sub directories to the project
# libraries
add_subdirectory(libs)

# unit testing
add_subdirectory(testing/unit)

# binaries (after `make install')
add_subdirectory(tools EXCLUDE_FROM_ALL)
add_subdirectory(apps EXCLUDE_FROM_ALL)
#add_subdirectory(validation EXCLUDE_FROM_ALL)

#------------------------------------------------------------------------------
# install config.h
install(FILES ${PROJECT_BINARY_DIR}/cug4config.h DESTINATION
        ${CMAKE_INSTALL_INCDIR}/cugeant4)
