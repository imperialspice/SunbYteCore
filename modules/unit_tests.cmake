
message("unit_tests.cmake")
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/unit_tests)
#targets setup
include_directories(${CMODULEPATH})
include_directories(${CMAKE_CURRENT_LIST_DIR}/core)
include_directories(${CMAKE_CURRENT_LIST_DIR}/client)
include_directories(${CMAKE_CURRENT_LIST_DIR}/tracking)
include_directories(${CMAKE_CURRENT_LIST_DIR}/sensors)

set(EXTERNAL ${CMAKE_CURRENT_LIST_DIR}/client/client.cpp ${CMAKE_CURRENT_LIST_DIR}/core/external.cpp )


add_executable(unitServer ${CMODULEPATH}/stressServer.cpp ${EXTERNAL})

add_executable(unitThread ${CMODULEPATH}/test_thread.cpp ${EXTERNAL})

add_executable(unitSettings ${CMODULEPATH}/unitSettings.cpp)

add_executable(unitMotors ${CMODULEPATH}/unitMotors.cpp ${CMAKE_CURRENT_LIST_DIR}/core/settings.cpp)

add_executable(unitBreak ${CMODULEPATH}/testBreak.cpp)

add_executable(unitSensors ${CMODULEPATH}/unitSensors.cpp ${CMAKE_CURRENT_LIST_DIR}/sensors/ina219.cc)

target_link_libraries(unitMotors)