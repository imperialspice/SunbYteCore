
message(${CMAKE_CURRENT_LIST_DIR})
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/core)
#targets setup

set(EXTERNAL ${CMODULEPATH}/external.cpp ${CMODULEPATH}/external.h ${CMODULEPATH}/internal.cpp ${CMODULEPATH}/internal.h)


add_executable(core ${CMODULEPATH}/main.cpp ${EXTERNAL})
target_link_libraries(core libpigpio.so)


set_target_properties(core
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )
