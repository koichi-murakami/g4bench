# executable
#------------------------------------------------------------------------------
set(APP vgeo)
#------------------------------------------------------------------------------
if(ENABLE_VIS)
  set(G4LIBS ${GEANT4_LIBRARIES_WITH_VIS})
else()
  set(G4LIBS ${GEANT4_LIBRARIES})
endif()

# executable
add_executable(${APP})

target_sources(${APP} PRIVATE
  appbuilder.cc calscorer.cc eventaction.cc main.cc medicalbeam.cc
  particlegun.cc phantom_pvp.cc runaction.cc simdata.cc
  stepaction.cc voxelgeom.cc
  ../util/jsonparser.cc
  ../util/stopwatch.cc
  ../util/timehistory.cc
)

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
install(TARGETS ${APP} DESTINATION ${CMAKE_INSTALL_PREFIX}/ecal)
install(FILES config.tmpl DESTINATION ${CMAKE_INSTALL_PREFIX}/ecal)
