# copy benchmark scripts to build directory

set(BENCH_SCRIPTS bench_all.sh ecal_e1000.sh hcal_p10.sh
                  vgeo_e20.sh vgeo_x18.sh)

file(COPY ${BENCH_SCRIPTS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(PROGRAMS ${BENCH_SCRIPTS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bench)
