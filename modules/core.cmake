
message(${CMAKE_CURRENT_LIST_DIR})
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/core)
#targets setup
include_directories(${CMODULEPATH})

set(EXTERNAL ${CMODULEPATH}/external.cpp ${CMODULEPATH}/internal.cpp ${CMODULEPATH}/external_i2c.cpp ${CMODULEPATH}/server.cpp
        )


add_executable(core ${CMODULEPATH}/server.cpp ${EXTERNAL})
target_link_libraries(core libpigpio.so _tracking.so ${OpenCV_LIBS} libspidev-lib++.a libspidev-lib.a)


set_target_properties(core
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )
