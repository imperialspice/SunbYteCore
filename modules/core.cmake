
message(${CMAKE_CURRENT_LIST_DIR})
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/core)
#targets setup
include_directories(${CMODULEPATH})

set(EXTERNAL ${CMODULEPATH}/external.cpp  ${CMODULEPATH}/server.cpp ${CMODULEPATH}/messages.cpp
        )


add_library(core ${CMODULEPATH}/server.cpp ${EXTERNAL})
#add_executable(core ${CMODULEPATH}/server.cpp ${EXTERNAL})
#target_link_libraries(core tracking ${OpenCV_LIBS} )


set_target_properties(core
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds"
        )
