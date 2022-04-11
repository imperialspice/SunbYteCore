set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0074 NEW)
cmake_minimum_required(VERSION 3.20)
project(sunbYte)



#get global includes
set(OpenCV_DIR deps/builds/opencv/lib/cmake/opencv4)
find_package(OpenCV REQUIRED NO_MODULE  NO_DEFAULT_PATH)

#tracking
include(modules/tracking.cmake)
include(modules/comms.cmake)
include(modules/core.cmake)
include(modules/speedtest.cmake)
