
message(${CMAKE_CURRENT_LIST_DIR})
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/test)
#targets setup
include_directories(${CMODULEPATH})

set(EXTERNAL)


add_executable(testBench ${CMODULEPATH}/testsmart.cpp ${EXTERNAL})
add_executable(testdeque ${CMODULEPATH}/testdeque.cpp)


