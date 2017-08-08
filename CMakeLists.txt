#==============================================================================
#  CMakeLists.txt for buidlig MPEXS
#
#  Copyright 2017 MPEXS Project
#
#  ***************************
#  !! DONOT EDIT THIS FILE  !!
#  ***************************
#==============================================================================
project(MPEXS)
#==============================================================================
cmake_minimum_required(VERSION 3.7)

include(config.cmake)

#------------------------------------------------------------------------------
# version.h generation (true for release)
if(DEVMODE)
  set(SKIP_VERSION FALSE)
else()
  set(SKIP_VERSION TRUE)
endif()

# static / shared library
set (LIBRARY_TYPE "STATIC")

# cmake modules
set(CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake/Modules
    ${CMAKE_MODULE_PATH})

# find packages
find_package(Geant4 REQUIRED)
find_package(CUDA 8.0)
find_package(Boost 1.53.0)
if(ENABLE_VIS)
find_package(GLFW3 3.2)
find_package(GLEW)
find_package(OpenGL)
endif()
if(DEVMODE)
find_package(CppUTest REQUIRED)
endif()

# c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# include path
if(NOT DEFINED CMAKE_INSTALL_INCDIR)
  set(CMAKE_INSTALL_INCDIR "include")
endif()

# CUDA flags
list(APPEND CUDA_NVCC_FLAGS "-std=c++11")

cuda_select_nvcc_arch_flags(NVCC_ARCH_FLAGS ${NVCC_ARCH})
list(APPEND CUDA_NVCC_FLAGS ${NVCC_ARCH_FLAGS})

if(NVCC_USE_FAST_MATH)
  list(APPEND CUDA_NVCC_FLAGS "--use_fast_math")
endif()

if(NVCC_ENABLE_L1_CACHE)
  list(APPEND CUDA_NVCC_FLAGS -Xptxas -dlcm=ca)
else()
  list(APPEND CUDA_NVCC_FLAGS -Xptxas -dlcm=cg)
endif()

if(NVCC_DEBUG)
  list(APPEND CUDA_NVCC_FLAGS -g -G -DTHRUST_DEBUG)
endif()

if(OPTIMIZE)
  list(APPEND CUDA_NVCC_FLAGS "-O3")
endif()

#set(CUDA_SEPARABLE_COMPILATION ON)

#------------------------------------------------------------------------------
# parameters for building
message(STATUS "--------------------------------------------------------")
message(STATUS "Parameters for building")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "NVCC_ARCH_FLAGS: ${NVCC_ARCH_FLAGS}")
if(ENABLE_VIS)
message(STATUS "GLFW3_FOUND: ${GLFW3_FOUND}")
message(STATUS "GLEW_FOUND: ${GLEW_FOUND}")
endif()
if(DEVMODE)
message(STATUS "CPPUTEST_FOUND: ${CPPUTEST_FOUND}")
endif()
message(STATUS "--------------------------------------------------------")

#------------------------------------------------------------------------------
# version.h (#.##d# + rev# / dev flag d/a/b/rc)
if(NOT SKIP_VERSION)
set(MPEXS_VERSION_MAJOR 2)
set(MPEXS_VERSION_MINOR 0d3)
execute_process (COMMAND git rev-parse --short HEAD OUTPUT_VARIABLE
               MPEXS_VERSION_BUILD OUTPUT_STRIP_TRAILING_WHITESPACE)
configure_file(
  "${PROJECT_SOURCE_DIR}/cmake/version.h.in"
  "${PROJECT_SOURCE_DIR}/mpexs/global/version.h"
)

install(FILES ${PROJECT_BINARY_DIR}/version.h DESTINATION
        ${CMAKE_INSTALL_INCDIR}/mpexs)
endif()
#------------------------------------------------------------------------------
# adding sub directories to the project
# libraries
add_subdirectory(libs)

# unit tests
if(DEVMODE)
include(CTest)
enable_testing()
add_subdirectory(tests/ut)
endif()

# integration tests
add_subdirectory(tests/devinfo EXCLUDE_FROM_ALL)
add_subdirectory(tests/ptrans EXCLUDE_FROM_ALL)

# binaries (after `make install')
