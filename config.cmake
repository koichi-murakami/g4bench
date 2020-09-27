#==============================================================================
# CMake configuration for G4Bench
#
# NOTE:
# CACHE variables can be changed in CMake CLI with -D option.
#==============================================================================
# installation prefix
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX $ENV{HOME}/work/g4bench/work
      CACHE STRING "Install prefix" FORCE)
endif()

# Geant4 installation path
set(GEANT4_INSTALL /opt/geant4/10.6.2
    CACHE STRING "Geant4 installation path")

# MT flag
set(ENABLE_MT FALSE CACHE BOOL "Enable multi-threading flag")

# visualization flag
set(ENABLE_VIS FALSE CACHE BOOL "Enable visualization flag")

# Optimizaton / Debug flags
set(OPTIMIZE TRUE CACHE BOOL "Optimizaton flag (O3)")
set(DEBUG FALSE CACHE BOOL "Debug mode")

# Development flag (set false for release)
set(DEVMODE FALSE CACHE BOOL "Development mode")

# Optional configurations
# Compiler
#set(CMAKE_C_COMPILER /opt/gcc-4.8.5/bin/gcc)
#set(CMAKE_CXX_COMPILER /opt/gcc-4.8.5/bin/g++)

# non-default Boost-C++
#set(BOOST_ROOT /opt/boost-1.60.0)
