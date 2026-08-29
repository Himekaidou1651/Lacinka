#!/usr/bin/env bash
set -euo pipefail

LAUNCHER_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$LAUNCHER_DIR/.." && pwd)"
cd "$ROOT_DIR"

g++ -std=c++17 \
  tools/transform_cli.cpp \
  core/transform/source.cpp \
  core/transform/hellas.cpp \
  core/transform/jugoslav.cpp \
  core/transform/choseon.cpp \
  core/transform/biela_old.cpp \
  core/transform/biela_new.cpp \
  core/transform/ukrajina.cpp \
  -o transform_cli
