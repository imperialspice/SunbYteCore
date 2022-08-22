set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0074 NEW)
cmake_minimum_required(VERSION 3.16.3)
project(sunbYte)

add_compile_options(-Wall)

link_libraries(pthread)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-debug-clang)

#get global includes
set(OpenCV_DIR deps/builds/opencv/lib/cmake/opencv4)
find_package(OpenCV REQUIRED NO_MODULE  NO_DEFAULT_PATH)
#get cereal
include_directories(deps/builds/cereal/include)
#get libgpiod
include_directories(deps/builds/libgpiod/include)
link_directories(deps/builds/libgpiod/lib)
# get spidev
include_directories(deps/builds/spidev/include)
link_directories(deps/builds/spidev/lib)
#lib(deps/builds/spidev/lib/libspidev-lib++.a deps/builds/spidev/lib/libspidev-lib.a)
# get libevent
include_directories(deps/builds/libevent/include)
link_directories(deps/builds/libevent/lib)
# get pigpio
include_directories(deps/builds/pigpio/include)
link_directories(deps/builds/pigpio/lib)


#tracking
include(modules/tracking.cmake)
include(modules/comms.cmake)
include(modules/core.cmake)
include(modules/speedtest.cmake)
include(modules/sensors.cmake)
include(modules/client.cmake)
include(modules/test.cmake)
include(modules/unit_tests.cmake)