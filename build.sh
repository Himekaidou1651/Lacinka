#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

mkdir -p tmp
pushd ui >/dev/null
windres resources.rc -O coff -o ../tmp/resources.o
popd >/dev/null

g++ -std=c++17 \
  ui/main.cpp \
  ui/js_runtime.cpp \
  core/transform/source.cpp \
  core/transform/hellas.cpp \
  core/transform/jugoslav.cpp \
  tmp/resources.o \
  -o Lacinka.exe \
  -municode -mwindows \
  -loleaut32 -lole32 -luuid \
  -lcomctl32 -lshell32 -lcomdlg32
