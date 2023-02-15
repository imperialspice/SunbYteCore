
message(${CMAKE_CURRENT_LIST_DIR})
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/rpi)
#targets setup
include_directories(${CMODULEPATH})
include_directories(${CMAKE_CURRENT_LIST_DIR}/core)

set(EXTERNAL ${CMAKE_CURRENT_LIST_DIR}/core/external.cpp ${CMAKE_CURRENT_LIST_DIR}/core/messages.cpp
        ${CMAKE_CURRENT_LIST_DIR}/core/settings.cpp
        )


add_executable(rpi_server ${CMODULEPATH}/rpi_server.cpp ${EXTERNAL})
target_link_libraries(rpi_server ${OpenCV_LIBS} core)


set_target_properties(rpi_server
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds"
        )
