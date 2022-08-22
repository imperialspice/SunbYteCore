
message("unit_tests.cmake")
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/unit_tests)
#targets setup
include_directories(${CMODULEPATH})
include_directories(${CMAKE_CURRENT_LIST_DIR}/core)
include_directories(${CMAKE_CURRENT_LIST_DIR}/client)

set(EXTERNAL ${CMAKE_CURRENT_LIST_DIR}/client/client.cpp ${CMAKE_CURRENT_LIST_DIR}/core/external.cpp )


add_executable(unitServer ${CMODULEPATH}/stressServer.cpp ${EXTERNAL})

add_executable(unitThread ${CMODULEPATH}/test_thread.cpp ${EXTERNAL})

