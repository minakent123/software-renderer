#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${REPO_ROOT}/build"

if ! command -v clang-tidy >/dev/null 2>&1; then
    echo "clang-tidy not found" >&2
    exit 1
fi

cmake -S "${REPO_ROOT}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON >/dev/null

if [ "$#" -gt 0 ]; then
    declare -a files=()
    for arg in "$@"; do
        if [[ "${arg}" = /* ]]; then
            files+=("${arg}")
        else
            files+=("${REPO_ROOT}/${arg}")
        fi
    done
else
    declare -a files=()
    while IFS= read -r file; do
        files+=("${REPO_ROOT}/${file}")
    done < <(cd "${REPO_ROOT}" && rg --files src -g '*.c' -g '*.cc' -g '*.cpp')
fi

if [ "${#files[@]}" -eq 0 ]; then
    echo "No source files found to analyze."
    exit 0
fi

clang-tidy -p "${BUILD_DIR}" "${files[@]}"
