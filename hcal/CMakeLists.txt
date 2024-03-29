# executable
#------------------------------------------------------------------------------
if(ENABLE_MT)
  set(APP hcal-mt)
else()
  set(APP hcal)
endif()
#------------------------------------------------------------------------------
if(ENABLE_VIS)
  set(G4LIBS ${GEANT4_LIBRARIES_WITH_VIS})
else()
  set(G4LIBS ${GEANT4_LIBRARIES})
endif()

# executable
add_executable(${APP})

target_sources(${APP} PRIVATE
  appbuilder.cc hcalgeom.cc main.cc
  ../common/calscorer.cc
  ../common/eventaction.cc
  ../common/g4environment.cc
  ../common/particlegun.cc
  ../common/runaction.cc
  ../common/stepaction.cc
  ../util/jsonparser.cc
  ../util/stopwatch.cc
  ../util/timehistory.cc
)

if(WIN32)
  target_sources(${APP} PRIVATE
    appbuilder.cc hcalgeom.cc main.cc
    ../common/calscorer.cc
    ../common/eventaction.cc
    ../common/g4environment.cc
    ../common/particlegun.cc
    ../common/runaction.cc
    ../common/stepaction.cc
    ../util/jsonparser.cc
    ../util/stopwatch.cc
    ../util/timehistory.cc
    ../util/getopt.c
    ../util/getopt_long.c
  )
endif()

if(ENABLE_MT)
  target_compile_definitions(${APP} PRIVATE ENABLE_MT)
endif()

if(ENABLE_VIS)
  target_compile_definitions(${APP} PRIVATE
    ENABLE_VIS G4INTY_USE_X G4VIS_USE_OPENGLX G4VIS_USE_OPENGLQT)
endif()

target_include_directories(${APP} PRIVATE
  ${PROJECT_SOURCE_DIR} ${GEANT4_INCLUDE_DIR})

target_link_directories(${APP} PRIVATE ${GEANT4_LIBRARY_DIR})

target_link_libraries(${APP} PRIVATE ${G4LIBS} PUBLIC global_cflags)

#
configure_file(config.tmpl g4bench.conf)

#
install(TARGETS ${APP} DESTINATION ${CMAKE_INSTALL_PREFIX}/hcal)
install(FILES config.tmpl RENAME g4bench.conf
        DESTINATION ${CMAKE_INSTALL_PREFIX}/hcal)
