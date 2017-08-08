#==============================================================================
# CMake configuration for MPEXS
#
# NOTE:
# CACHE variables can be changed in CMake CLI with -D option.
#==============================================================================
# Installation prefixes for MPEXS libraries
set(CMAKE_INSTALL_PREFIX $ENV{HOME}/Library/mpexs
    CACHE STRING "Install prefix")

# Geant4 installation path
set(GEANT4_INSTALL /opt/geant4/10.3.1
    CACHE STRING "Geant4 installation path")

# CUDA flags
set(NVCC_ARCH 6.0 6.1 CACHE STRING "CUDA compute capability")
set(NVCC_USE_FAST_MATH FALSE CACHE BOOL "fast-math flag")
set(NVCC_ENABLE_L1_CACHE FALSE CACHE BOOL "L1 chache usage flag")
set(NVCC_DEBUG FALSE CACHE BOOL "NVCC debug flag")

# visualization flag
set(ENABLE_VIS FALSE CACHE BOOL "Enable visualization flag")

# Optimizaton / Debug flags
set(OPTIMIZE TRUE CACHE BOOL "Optimizaton flag (O3)")
set(DEBUG FALSE CACHE BOOL "Debug mode")

# Development flag (set false for release)
set(DEVMODE TRUE CACHE BOOL "Development mode (Unit Testing)")
# Cpputest path for unit test
set(CPPUTEST_DIR /opt/cpputest)
