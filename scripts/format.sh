#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

if ! command -v clang-format >/dev/null 2>&1; then
    echo "clang-format not found" >&2
    exit 1
fi

declare -a filesToFormat=()
while IFS= read -r file; do
    filesToFormat+=("${REPO_ROOT}/${file}")
done < <(cd "${REPO_ROOT}" && rg --files src -g '*.c' -g '*.cc' -g '*.cpp' -g '*.h' -g '*.hpp')

if [ "${#filesToFormat[@]}" -eq 0 ]; then
    echo "No source files found to format."
    exit 0
fi

clang-format -i "${filesToFormat[@]}"
