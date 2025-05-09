cmake_minimum_required(VERSION 3.10)
project(rm_ui_generator)

set(CMAKE_CXX_STANDARD 20)

function(read_file path var)
    file(READ ${path} content)
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

if(CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?(.bat|.ps1|.py)?$")
    message(STATUS " * C++ compiler: Emscripten")
    set(CMAKE_CXX_COMPILER_ID "Emscripten")

    add_executable(
            rm_ui_generator
            designer.cpp
            objects/objects.cpp
            generators/dynamic.cpp
            generators/static.cpp
            utils/utils.cpp
    )
    target_compile_definitions(rm_ui_generator PUBLIC -DJSON_DISABLE_ENUM_SERIALIZATION=1)
    target_compile_options(rm_ui_generator PRIVATE -Oz)
    target_link_options(rm_ui_generator PUBLIC -lembind --emit-tsd generator.d.ts --extern-pre-js ${CMAKE_CURRENT_SOURCE_DIR}/utils/preLoad.js -Oz)
    target_include_directories(rm_ui_generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/inc)
else()
    message(STATUS " * C++ cimpiler: ${CMAKE_CXX_COMPILER_ID}")

    add_executable(
            rm_ui_generator
            main.cpp
            objects/objects.cpp
            generators/dynamic.cpp
            generators/static.cpp
            utils/utils.cpp
    )
    target_compile_definitions(rm_ui_generator PUBLIC -DJSON_DISABLE_ENUM_SERIALIZATION=1)
    target_include_directories(rm_ui_generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/inc)

    if (MSVC)
        target_compile_options(rm_ui_generator PRIVATE $<$<CONFIG:Release>:/MT> $<$<CONFIG:Debug>:/MTd>)
    elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
        target_link_options(rm_ui_generator PRIVATE -static -static-libgcc -static-libstdc++)
    endif ()
endif()


