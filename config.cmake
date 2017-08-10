#==============================================================================
# CMake configuration for G4Bench
#
# NOTE:
# CACHE variables can be changed in CMake CLI with -D option.
#==============================================================================
# Installation prefixes for MPEXS libraries
set(CMAKE_INSTALL_PREFIX $ENV{HOME}/Library/g4bench
    CACHE STRING "Install prefix")

# Geant4 installation path
set(GEANT4_INSTALL /opt/geant4/10.3.2
    CACHE STRING "Geant4 installation path")

# visualization flag
set(ENABLE_VIS TRUE CACHE BOOL "Enable visualization flag")

# Optimizaton / Debug flags
set(OPTIMIZE TRUE CACHE BOOL "Optimizaton flag (O3)")
set(DEBUG FALSE CACHE BOOL "Debug mode")

# Development flag (set false for release)
set(DEVMODE TRUE CACHE BOOL "Development mode (Unit Testing)")
