# Łacinka

[汉语](./README-zhCN.md)

Łacinka is a desktop transliteration tool built with Electron and a native C++ conversion core.
It takes text from the app UI or stdin and converts it into Latin-script variants for several languages.

## Features

- To Latin
- Electron UI with samples, counters, swap, copy, download, theme toggle, and always-on-top
- Native CLI converter reused by the desktop app

## Conversion Modes

| Mode | Source | Target |
| --- | --- | --- |
| 0 | Greek | Latin |
| 1 | Yugoslavian | Latin |
| 2 | Eonmon | Lumaja |
| 3 | Belarusian | Łacinka |
| 4 | Belarusian | 2007 Latin |
| 5 | Ukrainian | Latin |

## How It Works

The Electron front end sends text to `transform_cli`, a small C++ program that performs the transliteration.
The app is a UI shell around that native converter.

## Run

Start `Lacinka.exe`.

## Project Layout

- `main.js` - Electron main process
- `electron-start.js` - local launcher for development
- `frontend/` - renderer UI
- `core/transform/` - transliteration implementations
- `tools/transform_cli.cpp` - native command-line entry
- `launcher/` - build scripts
- `assets/icons/` - app icons

## Notes

- The UI supports sample text and exporting results as `.txt` or `.json`
- `Ctrl+Enter` triggers conversion in the desktop app
