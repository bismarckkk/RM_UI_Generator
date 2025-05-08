cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED ui_compile_generator)
    set(ui_compile_generator FALSE)
endif ()
if(NOT DEFINED ui_generator_bin)
    set(ui_output_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
endif ()

# 检测是否使用交叉编译工具链
function(check_baremetal_arm_compiler result)
    set(is_arm_compiler FALSE)

    if(CMAKE_C_COMPILER_ID MATCHES "ARMCC|ARMClang")
        set(is_arm_compiler TRUE)
    elseif(CMAKE_C_COMPILER MATCHES "arm-none-eabi-|-gnueabi-|armcc|armclang")
        set(is_arm_compiler TRUE)
    endif()

    set(${result} ${is_arm_compiler} PARENT_SCOPE)
endfunction()

function(setupGeneratorBuild)
    check_baremetal_arm_compiler(IS_BAREMETAL_ARM_COMPILER)

    # 如果使用交叉工具链则尝试为generator target应用宿主机工具链
    if(IS_BAREMETAL_ARM_COMPILER)
        message(STATUS "[UI Gen] Detected baremetal compiler. Try to use host compiler...")

        if(CMAKE_HOST_UNIX)
            set(DEFAULT_C_COMPILER "gcc")
            set(DEFAULT_CXX_COMPILER "g++")
            if(APPLE)
                set(DEFAULT_C_COMPILER "clang")
                set(DEFAULT_CXX_COMPILER "clang++")
            endif()
        elseif(CMAKE_HOST_WIN32)
            set(DEFAULT_C_COMPILER "gcc.exe")
            set(DEFAULT_CXX_COMPILER "g++.exe")
        endif()

        find_program(
                HOST_C_COMPILER_PATH
                NAMES ${DEFAULT_C_COMPILER}
                DOC "Host C compiler path"
        )

        find_program(
                HOST_CXX_COMPILER_PATH
                NAMES ${DEFAULT_CXX_COMPILER} cl.exe clang-cl.exe
                DOC "Host C++ compiler path"
        )

        set(HOST_C_COMPILER "${HOST_C_COMPILER_PATH}" CACHE FILEPATH "Host C compiler path")
        set(HOST_CXX_COMPILER "${HOST_CXX_COMPILER_PATH}" CACHE FILEPATH "Host C++ compiler path")

        # 验证编译器是否存在
        if(NOT HOST_C_COMPILER OR NOT EXISTS "${HOST_C_COMPILER}")
            message(FATAL_ERROR "[UI Gen] Cannot find host C compiler. Try to use -DHOST_C_COMPILER= parameter")
        endif()

        if(NOT HOST_CXX_COMPILER OR NOT EXISTS "${HOST_CXX_COMPILER}")
            message(FATAL_ERROR "[UI Gen] Cannot find host C++ compiler. Try to use -DHOST_CXX_COMPILER= parameter")
        endif()
    endif()

    include(ExternalProject)
    if (IS_BAREMETAL_ARM_COMPILER)
        message(STATUS [UI Gen] "Using host compiler: ${HOST_C_COMPILER} ${HOST_CXX_COMPILER}")
        ExternalProject_Add(
                ui_generator
                URL             <source_code_link>
                URL_HASH        SHA1=<source_code_sha1>
                PREFIX          ${CMAKE_CURRENT_BINARY_DIR}/generator
                BINARY_DIR      ${CMAKE_CURRENT_BINARY_DIR}/generator/bin
                CMAKE_ARGS
                    -DCMAKE_C_COMPILER=${HOST_C_COMPILER}
                    -DCMAKE_CXX_COMPILER=${HOST_CXX_COMPILER}
                    -DCMAKE_TOOLCHAIN_FILE=
                INSTALL_COMMAND ""
        )
    else ()
        ExternalProject_Add(
                ui_generator
                URL             <source_code_link>
                URL_HASH        SHA1=<source_code_sha1>
                PREFIX          ${CMAKE_CURRENT_BINARY_DIR}/generator
                BINARY_DIR      ${CMAKE_CURRENT_BINARY_DIR}/generator/bin
                INSTALL_COMMAND ""
        )
    endif ()
endfunction()

function(download_file url filename sha1)
    set(file_path "${CMAKE_CURRENT_BINARY_DIR}/${filename}")

    if(EXISTS "${file_path}")
        if(NOT "${sha1}" STREQUAL "")
            file(SHA1 "${file_path}" existing_sha1)
            if("${existing_sha1}" STREQUAL "${sha1}")
                message(STATUS "[UI Gen] File ${filename} exists with correct SHA1, skipping download")
                return()
            else()
                message(STATUS "[UI Gen] Removing invalid file (SHA1 mismatch): ${filename}")
                file(REMOVE "${file_path}")
            endif()
        else()
            message(STATUS "[UI Gen] File ${filename} exists (SHA1 check disabled), redownloading...")
            file(REMOVE "${file_path}")
        endif()
    endif()

    message(STATUS "[UI Gen] Downloading ${filename} from ${url}")
    file(DOWNLOAD
            "${url}"
            "${file_path}"
            SHOW_PROGRESS
            STATUS download_status
    )

    list(GET download_status 0 error_code)
    if(error_code)
        list(GET download_status 1 error_msg)
        file(REMOVE "${file_path}")
        message(FATAL_ERROR "[UI Gen] Download failed (${error_code}): ${error_msg}")
    endif()

    if(NOT "${sha1}" STREQUAL "")
        file(SHA1 "${file_path}" downloaded_sha1)
        if(NOT "${downloaded_sha1}" STREQUAL "${sha1}")
            file(REMOVE "${file_path}")
            message(FATAL_ERROR "[UI Gen] SHA1 verification failed:\n  Expected: ${sha1}\n  Actual: ${downloaded_sha1}")
        endif()
        message(STATUS "[UI Gen] SHA1 verification passed for ${filename}")
    endif()
endfunction()

function(verifyGenerator)
    execute_process(
            COMMAND ${ui_generator_bin} --test
            RESULT_VARIABLE TEST_RESULT
            OUTPUT_QUIET
            ERROR_QUIET
    )

    if(TEST_RESULT EQUAL 0)
        set(UI_GENERATOR_TEST_SUCCESS TRUE)
    else()
        message(FATAL_ERROR "[UI Gen] UI generator test failed (exit code: ${TEST_RESULT})")
    endif()
endfunction()

if(ui_compile_generator)
    setupGeneratorBuild()
    set(ui_generator_bin "${CMAKE_CURRENT_BINARY_DIR}/generator/bin/rm_ui_generator${CMAKE_EXECUTABLE_SUFFIX}")
    set(ui_compile_generator TRUE)
else()
    message(STATUS "${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64"))
        message(STATUS "[UI Gen] Detected Windows x64, using prebuild generator")
        download_file(
                <windows_download_link>
                "rm_ui_generator.exe"
                <windows_download_sha1>
        )
        set(ui_generator_bin "${CMAKE_CURRENT_BINARY_DIR}/rm_ui_generator.exe")
        verifyGenerator()
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64"))
        message(STATUS "[UI Gen] Detected Linux x64, using prebuild generator")
        download_file(
                <linux_download_link>
                "rm_ui_generator"
                <linux_download_sha1>
        )
        set(ui_generator_bin "${CMAKE_CURRENT_BINARY_DIR}/rm_ui_generator")
        verifyGenerator()
    else ()
        message(STATUS "[UI Gen] Detected unknown system. Building generator from source...")
        setupGeneratorBuild()
        set(ui_generator_bin "${CMAKE_CURRENT_BINARY_DIR}/generator/bin/rm_ui_generator${CMAKE_EXECUTABLE_SUFFIX}")
        set(ui_compile_generator TRUE)
    endif ()
endif()

function(add_ui TARGET_NAME MODE CONFIG_FILE)
    if(${ARGC} EQUAL 3)  # 没有传入frames参数
        set(FRAMES "default")
        message(STATUS "[UI Gen] Using default frame: ${FRAMES}")
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

    if(ui_compile_generator)
        set(generate_depend ui_generator)
    else()
        set(generate_depend ${ui_generator_bin})
    endif()

    add_custom_command(
            OUTPUT ${GENERATED_SOURCES}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${ui_output_dir}
            COMMAND ${ui_generator_bin}
                --input-file ${CONFIG_FILE}
                --output-dir ${ui_output_dir}/${TARGET_NAME}
                ${UI_STATIC}
                ${FRAMES}
            DEPENDS
                ${CONFIG_FILE}
                ${generate_depend}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    set_source_files_properties(${GENERATED_SOURCES} PROPERTIES GENERATED TRUE)

    add_library(${TARGET_NAME} STATIC
            ${GENERATED_SOURCES}
    )
    target_include_directories(${TARGET_NAME} PUBLIC
            ${ui_output_dir}/${TARGET_NAME}
    )

    if (ui_compile_generator)
        add_dependencies(${TARGET_NAME} ui_generator)
    endif ()
endfunction()
