
message(${CMAKE_CURRENT_LIST_DIR})
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/comms)
#targets setup
add_executable(mod_comms ${CMODULEPATH}/main.cpp)




set_target_properties(mod_tracking
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )
