cmake_minimum_required(VERSION 3.10)
project(rm_ui_generator)

set(CMAKE_CXX_STANDARD 20)

option(RM_UI_GENERATOR_BUILD_TESTS "Build test subdirectory" OFF)

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
    target_compile_options(rm_ui_generator PRIVATE -sNO_DISABLE_EXCEPTION_CATCHING -Oz)
    target_link_options(rm_ui_generator PUBLIC -lembind --extern-pre-js ${CMAKE_CURRENT_SOURCE_DIR}/utils/preLoad.js -sNO_DISABLE_EXCEPTION_CATCHING -Oz)
    target_include_directories(rm_ui_generator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/inc)
else()
    message(STATUS " * C++ cimpiler: ${CMAKE_CXX_COMPILER_ID}")

    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
        message(STATUS " * CMAKE_BUILD_TYPE not set, defaulting to Release")
    endif()

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

function(add_ui TARGET_NAME MODE CONFIG_FILE)
    if(${ARGC} EQUAL 3)
        set(FRAMES "default")
    else()
        set(FRAMES ${ARGN})
    endif()

    string(TOLOWER ${MODE} MODE)
    if (MODE STREQUAL "static")
        set(UI_STATIC "--static")
    elseif (MODE STREQUAL "dynamic")
        set(UI_STATIC "")
    else()
        message(FATAL_ERROR "[UI Gen] Invalid mode: ${MODE}. Use 'static' or 'dynamic'.")
    endif()

    if(NOT DEFINED ui_output_dir)
        set(ui_output_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
    endif()

    set(
            GENERATED_SOURCES
            ${ui_output_dir}/${TARGET_NAME}/ui.h
            ${ui_output_dir}/${TARGET_NAME}/ui_types.h
            ${ui_output_dir}/${TARGET_NAME}/ui_interface.h
            ${ui_output_dir}/${TARGET_NAME}/ui_interface.c
    )

    list(FIND FRAMES "default" DEFAULT_INDEX)
    list(LENGTH FRAMES FRAMES_LENGTH)
    if(DEFAULT_INDEX GREATER_EQUAL 0 AND FRAMES_LENGTH EQUAL 1)
        list(APPEND GENERATED_SOURCES
                ${ui_output_dir}/${TARGET_NAME}/ui_g.c
                ${ui_output_dir}/${TARGET_NAME}/ui_g.h
        )
    else()
        foreach(FRAME ${FRAMES})
            list(APPEND GENERATED_SOURCES
                    ${ui_output_dir}/${TARGET_NAME}/ui_${FRAME}.c
                    ${ui_output_dir}/${TARGET_NAME}/ui_${FRAME}.h
            )
        endforeach()
    endif()

    add_custom_command(
            OUTPUT ${GENERATED_SOURCES}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${ui_output_dir}
            COMMAND $<TARGET_FILE:rm_ui_generator>
                --input-file ${CONFIG_FILE}
                --output-dir ${ui_output_dir}/${TARGET_NAME}
                ${UI_STATIC}
                ${FRAMES}
            DEPENDS
                ${CONFIG_FILE}
                rm_ui_generator
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    set_source_files_properties(${GENERATED_SOURCES} PROPERTIES GENERATED TRUE)

    add_library(${TARGET_NAME} STATIC
            ${GENERATED_SOURCES}
    )
    target_include_directories(${TARGET_NAME} PUBLIC
            ${ui_output_dir}
    )
endfunction()

if(RM_UI_GENERATOR_BUILD_TESTS)
    add_subdirectory(test)
endif()
