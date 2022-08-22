
message(${CMAKE_CURRENT_LIST_DIR})
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/tracking)
#targets setup
add_executable(mod_tracking ${CMODULEPATH}/main.cpp ${CMODULEPATH}/main.hpp ${CMODULEPATH}/drv8711.cpp ${CMODULEPATH}/drv8711.h)
add_library(_tracking SHARED ${CMODULEPATH}/main.cpp ${CMODULEPATH}/main.hpp ${CMODULEPATH}/drv8711.cpp ${CMODULEPATH}/drv8711.h)


#SUNBYTE targets
include_directories(${OpenCV_INCLUDE_DIRS} )

target_link_libraries(mod_tracking ${OpenCV_LIBS} libspidev-lib++.a libspidev-lib.a)

set_target_properties(mod_tracking
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )
