# - Find Geant4 library
# This module sets up Geant4 information
# It defines:
# GEANT4_FOUND               If the Geant4 is found
# GEANT4_INCLUDE_DIR         PATH to the include directory
# GEANT4_LIBRARY_DIR         PATH to the library directory
# GEANT4_LIBRARIES           Most common libraries
# GEANT4_LIBRARIES_WITH_VIS  Most common libraries with visualization
# GEANT4_MT                  Availabiliy of MT capability (bool)
# GEANT4_VIS                 Availabiliy of Vis package (bool)

find_program(GEANT4_CONFIG NAMES geant4-config
             PATHS $ENV{GEANT4_INSTALL}/bin
                   ${GEANT4_INSTALL}/bin
                   /usr/local/bin /opt/local/bin)

if(GEANT4_CONFIG)
  set(GEANT4_FOUND TRUE)

  execute_process(COMMAND ${GEANT4_CONFIG} --prefix
                  OUTPUT_VARIABLE GEANT4_PREFIX
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(COMMAND ${GEANT4_CONFIG} --version
                  OUTPUT_VARIABLE GEANT4_VERSION
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(COMMAND ${GEANT4_CONFIG} --has-feature multithreading
                  OUTPUT_VARIABLE _GEANT4_MT_YES_OR_NO
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(COMMAND ${GEANT4_CONFIG} --has-feature opengl-x11
                  OUTPUT_VARIABLE _GEANT4_VIS_YES_OR_NO
                  OUTPUT_STRIP_TRAILING_WHITESPACE)

  if ( ${_GEANT4_MT_YES_OR_NO} MATCHES "yes" )
    set(GEANT4_MT true)
  else()
    set(GEANT4_MT false)
  endif()

  if ( ${_GEANT4_VIS_YES_OR_NO} MATCHES "yes" )
    set(GEANT4_VIS true)
  else()
    set(GEANT4_VIS false)
  endif()

elseif(WIN32)
   find_file(GEANT4_WIN NAMES G4run.dll PATHS ${GEANT4_INSTALL}/bin)

  if(GEANT4_WIN)
    set(GEANT4_PREFIX ${GEANT4_INSTALL})
    set(GEANT4_FOUND TRUE)
    set(GEANT4_MT FALSE)
    set(GEANT4_VIS FALSE)

  else()
    set(GEANT4_FOUND FALSE)
    if (Geant4_FIND_REQUIRED)
      message(FATAL_ERROR "NOT Found Geant4: set GEANT4_INSTALL.")
    else()
      message(WARNING "NOT Found Geant4: set GEANT4_INSTALL.")
    endif()
  endif()
 
else()
  set(GEANT4_FOUND FALSE)
  if (Geant4_FIND_REQUIRED)
    message(FATAL_ERROR "NOT Found Geant4: set GEANT4_INSTALL.")
  else()
    message(WARNING "NOT Found Geant4: set GEANT4_INSTALL.")
  endif()

endif()

message(STATUS "Found Geant4: ${GEANT4_PREFIX} (${GEANT4_VERSION})")
message(STATUS "Geant4 MT: ${GEANT4_MT}")
message(STATUS "Geant4 Visualization: ${GEANT4_VIS}")

set(GEANT4_INCLUDE_DIR ${GEANT4_PREFIX}/include/Geant4)
if(WIN32)
  set(GEANT4_LIBRARY_DIR ${GEANT4_PREFIX}/lib)
else()
  set(GEANT4_LIBRARY_DIR ${GEANT4_PREFIX}/${_LIBDIR_DEFAULT})
endif()

if (GEANT4_STATIC)
if(WIN32)
  set(GEANT4_LIBRARIES  G4interfaces-static G4persistency-static G4analysis-static
                        G4error_propagation-static G4readout-static G4physicslists-static
                        G4run-static G4event-static G4tracking-static G4parmodels-static G4processes-static
                        G4digits_hits-static G4track-static G4particles-static G4geometry-static
                        G4materials-static G4graphics_reps-static G4intercoms-static
                        G4global-static G4clhep-static G4expat-static G4zlib-static)
else()
  set(GEANT4_LIBRARIES  ${GEANT4_LIBRARY_DIR}/libG4interfaces.a
                        ${GEANT4_LIBRARY_DIR}/libG4persistency.a
                        ${GEANT4_LIBRARY_DIR}/libG4analysis.a
                        ${GEANT4_LIBRARY_DIR}/libG4error_propagation.a
                        ${GEANT4_LIBRARY_DIR}/libG4readout.a
                        ${GEANT4_LIBRARY_DIR}/libG4physicslists.a
                        ${GEANT4_LIBRARY_DIR}/libG4run.a
                        ${GEANT4_LIBRARY_DIR}/libG4event.a
                        ${GEANT4_LIBRARY_DIR}/libG4tracking.a
                        ${GEANT4_LIBRARY_DIR}/libG4parmodels.a
                        ${GEANT4_LIBRARY_DIR}/libG4processes.a
                        ${GEANT4_LIBRARY_DIR}/libG4digits_hits.a
                        ${GEANT4_LIBRARY_DIR}/libG4track.a
                        ${GEANT4_LIBRARY_DIR}/libG4particles.a
                        ${GEANT4_LIBRARY_DIR}/libG4geometry.a
                        ${GEANT4_LIBRARY_DIR}/libG4materials.a
                        ${GEANT4_LIBRARY_DIR}/libG4graphics_reps.a
                        ${GEANT4_LIBRARY_DIR}/libG4intercoms.a
                        ${GEANT4_LIBRARY_DIR}/libG4global.a
                        ${GEANT4_LIBRARY_DIR}/libG4clhep.a
                        ${GEANT4_LIBRARY_DIR}/libG4zlib.a
                        expat pthread)
 endif()

else()
set(GEANT4_LIBRARIES  G4interfaces G4persistency G4analysis
                      G4error_propagation G4readout G4physicslists
                      G4run G4event G4tracking G4parmodels G4processes
                      G4digits_hits G4track G4particles G4geometry
                      G4materials G4graphics_reps G4intercoms
                      G4global G4clhep G4zlib)

set(GEANT4_LIBRARIES_WITH_VIS
                      G4OpenGL G4gl2ps G4Tree G4FR G4GMocren G4visHepRep
                      G4RayTracer G4VRML G4vis_management G4modeling
                      G4interfaces G4persistency G4analysis
                      G4error_propagation G4readout G4physicslists
                      G4run G4event G4tracking G4parmodels G4processes
                      G4digits_hits G4track G4particles G4geometry
                      G4materials G4graphics_reps G4intercoms
                      G4global G4clhep G4zlib)

endif()
