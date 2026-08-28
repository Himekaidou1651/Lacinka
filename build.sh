#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

g++ -std=c++17 \
  ui/main.cpp \
  core/transform/source.cpp \
  core/transform/hellas.cpp \
  core/transform/jugoslav.cpp \
  -o Lacinka.exe \
  -municode -mwindows \
  -lcomctl32 -lshell32
