
message(${CMAKE_CURRENT_LIST_DIR})
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/sensors)
#targets setup
add_executable(mod_sensors ${CMODULEPATH}/sensors.cpp ${CMODULEPATH}/i2c.c ${CMODULEPATH}/bno055.cpp ${CMODULEPATH}/i2c_bno055.cpp)

target_link_libraries(mod_sensors libspidev-lib++.a)


set_target_properties(mod_sensors
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds"
        )

