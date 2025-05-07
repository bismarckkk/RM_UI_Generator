cmake_minimum_required(VERSION 3.10)
project(rm_ui_generator)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        rm_ui_generator
        main.cpp
        objects/objects.cpp
        generators/dynamic.cpp
        generators/static.cpp
)
target_compile_definitions(rm_ui_generator PUBLIC -DJSON_DISABLE_ENUM_SERIALIZATION=1)
target_include_directories(rm_ui_generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
