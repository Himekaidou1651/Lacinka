#!/usr/bin/env bash
set -euo pipefail

LAUNCHER_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$LAUNCHER_DIR/.." && pwd)"
cd "$ROOT_DIR"

"$LAUNCHER_DIR/build-transform-cli.sh"

export CSC_IDENTITY_AUTO_DISCOVERY=false
npm run dist

if [[ -f "dist/Lacinka.exe" ]]; then
  cp -f "dist/Lacinka.exe" "Lacinka.exe"
else
  echo "Build failed: dist/Lacinka.exe was not found." >&2
  exit 1
fi

echo "Build complete: $ROOT_DIR/Lacinka.exe"
