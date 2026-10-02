
cmake_minimum_required(VERSION 3.22)

project(CPvP_AutoTotemLEVI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Official LeviLauncher Preloader SDK
FetchContent_Declare(
    preloader_android
    GIT_REPOSITORY https://github.com/LiteLDev/preloader-android.git
    GIT_TAG 0.2.2
)
FetchContent_MakeAvailable(preloader_android)

# Dependencies
set(MOD_DEPS)

if(NOT TARGET nlohmann_json::nlohmann_json)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
    )
    list(APPEND MOD_DEPS nlohmann_json)
endif()

if(NOT TARGET Boost::pfr)
    FetchContent_Declare(
        boost_pfr
        GIT_REPOSITORY https://github.com/boostorg/pfr.git
        GIT_TAG boost-1.88.0
    )
    list(APPEND MOD_DEPS boost_pfr)
endif()

if(NOT TARGET magic_enum::magic_enum)
    FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG v0.9.7
    )
    list(APPEND MOD_DEPS magic_enum)
endif()

if(NOT TARGET fmt::fmt)
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 11.2.0
    )
    set(FMT_DOC OFF CACHE BOOL "" FORCE)
    set(FMT_TEST OFF CACHE BOOL "" FORCE)
    set(FMT_INSTALL OFF CACHE BOOL "" FORCE)
    list(APPEND MOD_DEPS fmt)
endif()

if(MOD_DEPS)
    FetchContent_MakeAvailable(${MOD_DEPS})
endif()

function(configure_mod target)
    target_include_directories(${target} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${preloader_android_SOURCE_DIR}/include"
    )

    target_link_libraries(${target} PRIVATE
        nlohmann_json::nlohmann_json
        Boost::pfr
        magic_enum::magic_enum
        fmt::fmt
    )

    target_compile_options(${target} PRIVATE
        -Wall -Wextra -Wpedantic
    )
endfunction()

# Config generator
if(NOT ANDROID)
    add_executable(config_gen
        src/GenerateConfig.cpp
    )

    configure_mod(config_gen)

    target_compile_definitions(config_gen PRIVATE
        PL_CONFIG_NO_RUNTIME
    )
endif()

# Android native mod
if(ANDROID)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
        "${CMAKE_BINARY_DIR}/out/${ANDROID_ABI}"
    )

    find_library(ANDROID_LOG_LIB log)

    add_library(cpvp_autototem SHARED
        src/FullCppMod.cpp
    )

    set_target_properties(cpvp_autototem PROPERTIES
        OUTPUT_NAME "CPvPAutoTotem"
    )

    configure_mod(cpvp_autototem)

    target_link_libraries(cpvp_autototem PRIVATE
        preloader
    )

    if(ANDROID_LOG_LIB)
        target_link_libraries(cpvp_autototem PRIVATE
            "${ANDROID_LOG_LIB}"
        )
    endif()
endif()
