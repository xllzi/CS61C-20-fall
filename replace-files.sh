#!/usr/bin/env bash
# Usage: ./replace-file.sh <source-file> <search-root>


set -euo pipefail

SRC="$1"
ROOT="${2:-.}"         # default search root is current directory
BASENAME="$(basename "$SRC")"

# Find all files with the same name under ROOT
find "$ROOT" -type f -name "$BASENAME" | while IFS= read -r target; do
    echo "Replacing: $target"
    cp -f "$SRC" "$target"
done
