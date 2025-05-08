cmake_minimum_required(VERSION 3.10)
project(rm_ui_generator_test)

set(addUIPath "${CMAKE_CURRENT_BINARY_DIR}/addUI.cmake")
file(DOWNLOAD
        https://github.com/bismarckkk/RM_UI_Generator/releases/latest/download/addUI.cmake
        ${addUIPath}
)
include(${addUIPath})

set(ui_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/dynamic")
add_ui(dynamic_ui DYNAMIC ui.rmui)

set(ui_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/static")
add_ui(static_ui STATIC ui.rmui)

add_library(serialib utils/serialib.cpp)
target_include_directories(serialib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils)
target_link_libraries(dynamic_ui serialib)
target_link_libraries(static_ui serialib)

add_executable(dynamic dynamic/main.cpp)
target_link_libraries(dynamic dynamic_ui)

add_executable(static static/main.cpp)
target_link_libraries(static static_ui)
