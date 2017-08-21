# executable
#------------------------------------------------------------------------------
set(APP_NAME ecal)
#------------------------------------------------------------------------------
include_directories(${PROJECT_SOURCE_DIR} ${GEANT4_INCLUDE_DIR})

link_directories(${GEANT4_LIBRARY_DIR})

if(ENABLE_VIS)
  add_definitions(-DG4INTY_USE_X -DG4VIS_USE_OPENGLX -DG4VIS_USE_OPENGLQT)
  add_definitions(-DENABLE_VIS)
endif()

# executable
add_executable(${APP_NAME}
  appbuilder.cc calscorer.cc ecalgeom.cc eventaction.cc main.cc
  particlegun.cc runaction.cc simdata.cc stepaction.cc
  ../util/jsonparser.cc
  ../util/stopwatch.cc
  ../util/timehistory.cc
)

if(ENABLE_VIS)
  target_link_libraries(${APP_NAME} ${GEANT4_LIBRARIES_WITH_VIS})
else()
  target_link_libraries(${APP_NAME} ${GEANT4_LIBRARIES})
endif()

set_target_properties(${APP_NAME}
                      PROPERTIES INSTALL_RPATH
                      ${GEANT4_LIBRARY_DIR}
                      BUILD_WITH_INSTALL_RPATH TRUE)

install(TARGETS ${APP_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/ecal)
install(FILES config.tmpl DESTINATION ${CMAKE_INSTALL_PREFIX}/ecal)
