#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${REPO_ROOT}/build"
BUILD_TYPE_INPUT="${1:-debug}"

case "${BUILD_TYPE_INPUT}" in
    debug|Debug)
        BUILD_TYPE="Debug"
        ;;
    release|Release)
        BUILD_TYPE="Release"
        ;;
    *)
        echo "Usage: $0 [debug|release]" >&2
        exit 1
        ;;
esac

cmake -S "${REPO_ROOT}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
cmake --build "${BUILD_DIR}" --config "${BUILD_TYPE}"
