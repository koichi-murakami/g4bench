#
#  CMakeLists.txt for buidlig G4Bench
#
#  Copyright 2017 Koichi Murakami
#
#  ***************************
#  !! DONOT EDIT THIS FILE  !!
#  ***************************
#
#==============================================================================
project(G4Bench)
#==============================================================================
cmake_minimum_required(VERSION 3.11)
include(config.cmake)

# version.h generation (true for release)
if(DEVMODE)
  option(SKIP_VERSION "overwrite version.h" OFF)
else()
  option(SKIP_VERSION "overwrite version.h" ON)
endif()

# cmake modules
set(CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake/Modules
    ${CMAKE_MODULE_PATH})

# library path (lib or lib64)
if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(_LIBDIR_DEFAULT "lib")
  if(CMAKE_SYSTEM_NAME MATCHES "Linux"
      AND NOT EXISTS "/etc/debian_version")
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
      set(_LIBDIR_DEFAULT "lib64")
    endif()
  endif()
  set(CMAKE_INSTALL_LIBDIR "${_LIBDIR_DEFAULT}")
endif()

# find packages
find_package(Geant4 REQUIRED)
find_package(Boost 1.53.0)
find_package(OpenGL)

# c++11
add_library(global_cflags INTERFACE)
target_compile_features(global_cflags INTERFACE cxx_std_11 )
set(CMAKE_CXX_EXTENSIONS OFF)

# build options
if(NOT CMAKE_BUILD_TYPE)
  if(DEBUG)
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
  elseif(OPTIMIZE)
    set(CMAKE_BUILD_TYPE "Release")
  else()
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
  endif()
endif()

# Geant4 visualization
if( NOT GEANT4_VIS)
  set (ENABLE_VIS FALSE)
endif()

# rpath setting
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--disable-new-dtags")

#------------------------------------------------------------------------------
# parameters for building
message(STATUS "--------------------------------------------------------")
message(STATUS "Parameters for building")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "GEANT4_LIBRARY_DIR: ${GEANT4_LIBRARY_DIR}")

message(STATUS "--------------------------------------------------------")

#------------------------------------------------------------------------------
# version.h (#.##d# + rev# / dev flag d/a/b/rc)
if(NOT SKIP_VERSION)
set(G4BENCH_VERSION_MAJOR 1)
set(G4BENCH_VERSION_MINOR 4.0d1)
execute_process (COMMAND git rev-parse --short HEAD OUTPUT_VARIABLE
                 G4BENCH_VERSION_BUILD OUTPUT_STRIP_TRAILING_WHITESPACE)
configure_file(
  "${PROJECT_SOURCE_DIR}/cmake/version.h.in"
  "${PROJECT_SOURCE_DIR}/version.h"
)
endif()

#------------------------------------------------------------------------------
# adding sub directories to the project
add_subdirectory(ecal)
add_subdirectory(hcal)
add_subdirectory(vgeo)
