
message(${CMAKE_CURRENT_LIST_DIR})
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/client)
#targets setup
include_directories(${CMAKE_CURRENT_LIST_DIR}/core)
set(AdditionalFiles ${CMAKE_CURRENT_LIST_DIR}/core/external.cpp ${CMAKE_CURRENT_LIST_DIR}/core/messages.cpp ${CMAKE_CURRENT_LIST_DIR}/core/settings.cpp)


add_executable(client ${CMODULEPATH}/run.cpp ${CMODULEPATH}/client.cpp ${CMODULEPATH}/client.h ${AdditionalFiles})


add_executable(downlink ${CMODULEPATH}/downlink.cpp ${CMODULEPATH}/client.cpp ${CMODULEPATH}/client.h ${AdditionalFiles})

target_link_libraries(client )




set_target_properties(client
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/builds"
        )
