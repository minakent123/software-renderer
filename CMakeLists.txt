cmake_minimum_required(VERSION 3.20)

project(software_renderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT APPLE)
    message(FATAL_ERROR "This project currently expects prebuilt SDL2 only on macOS.")
endif()

set(SDL2_PREBUILT_PLATFORM "macos")

if(CMAKE_OSX_ARCHITECTURES)
    list(GET CMAKE_OSX_ARCHITECTURES 0 SDL2_PREBUILT_ARCH)
else()
    set(SDL2_PREBUILT_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
endif()

if(SDL2_PREBUILT_ARCH STREQUAL "aarch64")
    set(SDL2_PREBUILT_ARCH "arm64")
endif()

set(SDL2_PREBUILT_ROOT
    "${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/SDL2/${SDL2_PREBUILT_PLATFORM}/${SDL2_PREBUILT_ARCH}")
set(SDL2_INCLUDE_DIR "${SDL2_PREBUILT_ROOT}/include")
set(SDL2_RELEASE_LIB "${SDL2_PREBUILT_ROOT}/release/lib/libSDL2.a")
set(SDL2_DEBUG_LIB "${SDL2_PREBUILT_ROOT}/debug/lib/libSDL2d.a")

foreach(path IN ITEMS "${SDL2_INCLUDE_DIR}" "${SDL2_RELEASE_LIB}" "${SDL2_DEBUG_LIB}")
    if(NOT EXISTS "${path}")
        message(FATAL_ERROR "Missing prebuilt SDL2 asset: ${path}")
    endif()
endforeach()

set(SDL2_MACOS_SYSTEM_LIBS
    m
    "-framework CoreVideo"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework ForceFeedback"
    "-framework Carbon"
    "-framework CoreAudio"
    "-framework AudioToolbox"
    "-framework AVFoundation"
    "-framework Foundation"
    "-weak_framework GameController"
    "-weak_framework Metal"
    "-weak_framework QuartzCore"
    "-weak_framework CoreHaptics"
)

add_library(SDL2::SDL2 STATIC IMPORTED GLOBAL)
set_target_properties(SDL2::SDL2 PROPERTIES
    IMPORTED_LOCATION "${SDL2_RELEASE_LIB}"
    IMPORTED_LOCATION_DEBUG "${SDL2_DEBUG_LIB}"
    IMPORTED_LOCATION_RELEASE "${SDL2_RELEASE_LIB}"
    IMPORTED_LOCATION_RELWITHDEBINFO "${SDL2_RELEASE_LIB}"
    IMPORTED_LOCATION_MINSIZEREL "${SDL2_RELEASE_LIB}"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
    INTERFACE_LINK_LIBRARIES "${SDL2_MACOS_SYSTEM_LIBS}"
)

add_executable(software_renderer
    src/main.cpp
)
target_link_libraries(software_renderer PRIVATE SDL2::SDL2)
