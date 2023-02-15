
message(${CMAKE_CURRENT_LIST_DIR})
set(CMODULEPATH ${CMAKE_CURRENT_LIST_DIR}/camera_primary)
#targets setup
include_directories(${CMAKE_CURRENT_LIST_DIR}/core)
set(AdditionalFiles ${CMAKE_CURRENT_LIST_DIR}/core/messages.cpp ${CMAKE_CURRENT_LIST_DIR}/core/settings.cpp)


add_executable(primary_camera ${CMODULEPATH}/main.cpp ${CMODULEPATH}/main.h ${AdditionalFiles})

target_link_libraries(primary_camera libASICamera.a libusb-1.0.so)

add_library(science_lib ${CMODULEPATH}/main.cpp ${CMODULEPATH}/main.h ${AdditionalFiles})

target_link_libraries(science_lib libASICamera2.a libusb-1.0.so)



