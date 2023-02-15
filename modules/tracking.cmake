
message(${CMAKE_CURRENT_LIST_DIR})
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/tracking)
#targets setup
add_executable(mod_tracking ${CMODULEPATH}/main.cpp ${CMODULEPATH}/main.hpp ${CMAKE_CURRENT_LIST_DIR}/core/settings.cpp ${CMAKE_CURRENT_LIST_DIR}/core/messages.cpp)
add_library(tracking SHARED ${CMODULEPATH}/main.cpp ${CMODULEPATH}/main.hpp ${CMAKE_CURRENT_LIST_DIR}/core/settings.cpp ${CMAKE_CURRENT_LIST_DIR}/core/messages.cpp)


#SUNBYTE targets
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(mod_tracking ${OpenCV_LIBS} )
target_link_libraries(tracking ${OpenCV_LIBS} )

set_target_properties(mod_tracking
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds"
        )

set_target_properties(tracking
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds"
        )
