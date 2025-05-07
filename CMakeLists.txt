cmake_minimum_required(VERSION 3.10)
project(rm_ui_generator)

set(CMAKE_CXX_STANDARD 20)

function(read_file path var)
    file(READ ${path} content)
#    string(REPLACE "\\" "\\\\"  content "${content}")
    set(${var} "${content}" PARENT_SCOPE)
endfunction()

read_file(${CMAKE_CURRENT_SOURCE_DIR}/lib/dynamic/ui_types.h UI_TYPES_H_DYNAMIC_CONTENT)
read_file(${CMAKE_CURRENT_SOURCE_DIR}/lib/dynamic/ui_interface.h UI_INTERFACE_H_DYNAMIC_CONTENT)
read_file(${CMAKE_CURRENT_SOURCE_DIR}/lib/dynamic/ui_interface.c UI_INTERFACE_C_DYNAMIC_CONTENT)
read_file(${CMAKE_CURRENT_SOURCE_DIR}/lib/static/ui_types.h UI_TYPES_H_STATIC_CONTENT)
read_file(${CMAKE_CURRENT_SOURCE_DIR}/lib/static/ui_interface.h UI_INTERFACE_H_STATIC_CONTENT)
read_file(${CMAKE_CURRENT_SOURCE_DIR}/lib/static/ui_interface.c UI_INTERFACE_C_STATIC_CONTENT)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/lib.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/inc/lib.hpp
)

add_executable(
        rm_ui_generator
        main.cpp
        objects/objects.cpp
        generators/dynamic.cpp
        generators/static.cpp
)
target_compile_definitions(rm_ui_generator PUBLIC -DJSON_DISABLE_ENUM_SERIALIZATION=1)
target_include_directories(rm_ui_generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/inc)
