# executable
#------------------------------------------------------------------------------
set(APP_NAME ecal)
#------------------------------------------------------------------------------
include_directories(${PROJECT_SOURCE_DIR} ${GEANT4_INCLUDE_DIR})

link_directories(${GEANT4_LIBRARY_DIR})

# executable
add_executable(${APP_NAME}
  appbuilder.cc geometry.cc main.cc
  ../util/jsonparser.cc
  ../util/stopwatch.cc
  ../util/timehistory.cc
)

target_link_libraries(${APP_NAME} ${GEANT4_LIBRARIES_WITH_VIS} z)

set_target_properties(${APP_NAME}
                      PROPERTIES INSTALL_RPATH
                      ${GEANT4_LIBRARY_DIR}
                      BUILD_WITH_INSTALL_RPATH TRUE)

install(TARGETS ${APP_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES config.json DESTINATION ${CMAKE_INSTALL_PREFIX}/share)
