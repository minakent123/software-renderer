#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
SDL_SOURCE_DIR="${REPO_ROOT}/third_party/SDL2"
BUILD_ROOT="${REPO_ROOT}/build"
OUTPUT_ROOT="${REPO_ROOT}/prebuilt/SDL2/macos/arm64"

build_sdl2() {
    local config="$1"
    local build_dir="$2"
    local output_lib="$3"
    local output_dir=""

    case "${config}" in
        Release) output_dir="release" ;;
        Debug) output_dir="debug" ;;
        *)
            echo "Unsupported config: ${config}" >&2
            exit 1
            ;;
    esac

    cmake -S "${SDL_SOURCE_DIR}" -B "${build_dir}" \
        -DSDL_SHARED=OFF \
        -DSDL_STATIC=ON \
        -DSDL_TEST=OFF \
        -DCMAKE_BUILD_TYPE="${config}"

    cmake --build "${build_dir}" --target SDL2-static -j8

    cp "${output_lib}" "${OUTPUT_ROOT}/${output_dir}/lib/"
}

mkdir -p "${OUTPUT_ROOT}/include"
mkdir -p "${OUTPUT_ROOT}/debug/lib"
mkdir -p "${OUTPUT_ROOT}/release/lib"

build_sdl2 "Release" "${BUILD_ROOT}/prebuilt-sdl2-release" "${BUILD_ROOT}/prebuilt-sdl2-release/libSDL2.a"
build_sdl2 "Debug" "${BUILD_ROOT}/prebuilt-sdl2-debug" "${BUILD_ROOT}/prebuilt-sdl2-debug/libSDL2d.a"

cp -R "${BUILD_ROOT}/prebuilt-sdl2-release/include/." "${OUTPUT_ROOT}/include/"
cp "${SDL_SOURCE_DIR}/include/SDL_config.h" "${OUTPUT_ROOT}/include/"
cp "${SDL_SOURCE_DIR}/include/SDL_config_macosx.h" "${OUTPUT_ROOT}/include/"
