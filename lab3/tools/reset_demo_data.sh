#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(cd -- "${SCRIPT_DIR}/.." && pwd)"

if pgrep -f "${PROJECT_DIR}/build/warehouse_app" >/dev/null 2>&1; then
    echo "Close warehouse_app before resetting its data." >&2
    exit 1
fi

find "${PROJECT_DIR}/data" -maxdepth 1 -type f \( -name '*.csv' -o -name '*.tmp' \) -delete

echo "Working data removed. The next launch will load data/demo/*.csv."
