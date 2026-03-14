#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

if ! command -v clang-format >/dev/null 2>&1; then
    echo "clang-format not found" >&2
    exit 1
fi

declare -a FILES=()

if command -v rg >/dev/null 2>&1; then
    while IFS= read -r file; do
        FILES+=("${REPO_ROOT}/${file}")
    done < <(cd "${REPO_ROOT}" && rg --files src -g '*.c' -g '*.cc' -g '*.cpp' -g '*.h' -g '*.hpp')
else
    while IFS= read -r file; do
        FILES+=("${file}")
    done < <(find "${REPO_ROOT}/src" \( -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -type f)
fi

if [ "${#FILES[@]}" -eq 0 ]; then
    echo "No source files found to format."
    exit 0
fi

clang-format -i "${FILES[@]}"
