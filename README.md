# Łacinka

[汉语](./README-zhCN.md)

Łacinka is a desktop transliteration tool built with Electron and a native C++ conversion core.
It converts text into Latin-script variants for several languages, either through the GUI or from standard input via the command line.

## Features

- 8 transliteration modes (Greek, Serbo-Croatian, Choseoneo, Belarusian, Ukrainian, Latin, Russian)
- Frameless Electron window with a custom title bar (minimize / maximize / close)
- Input panel with live character count and a length meter
- Mode-aware "Insert sample" button — each mode inserts a different sample text
- Swap input ↔ output, copy output to clipboard, download result as `.txt` or `.json`
- Light/dark theme toggle and always-on-top toggle
- Chinese/English interface switch (i18n)
- Status bar showing current status and last-run time; error banner and toast notifications
- Keyboard shortcuts: `Ctrl+Enter` runs the conversion, `Esc` closes the download menu
- Native CLI converter (`transform_cli`) reused by the desktop app

## Conversion Modes

| Mode | Source | Target |
| --- | --- | --- |
| 0 | Greek | Latin |
| 1 | Yugoslavian | Latin |
| 2 | Eonmon | Lumaja |
| 3 | Belarusian | Łacinka |
| 4 | Belarusian | 2007 Latin |
| 5 | Ukrainian | Łacinka |
| 6 | Latina | Ecclesiasticum |
| 7 | Russian | Łacinka |
| 8 | Russian | Old Łacinka |

## How It Works

The Electron front end sends text to `transform_cli`, a small C++ program that performs the transliteration.
The app is a UI shell around that native converter.

## Run

Start `Lacinka.exe`.

## Project Layout

- `main.js` - Electron main process (window management, IPC, spawns `transform_cli`)
- `electron-start.js` - local development launcher
- `frontend/` - renderer UI
  - `index.html`, `style.css`, `renderer.js` - interface and behavior
  - `preload.js` - context bridge (window controls + transform)
  - `i18n/` - zh-CN and en language dictionaries
- `core/common/Common.js` - shared sample texts and config (char limits, toast duration)
- `core/transform/` - transliteration implementations (C++)
- `tools/transform_cli.cpp` - native command-line entry
- `launcher/` - build scripts
- `assets/icons/` - app icons

## Notes

- The character counter turns to a warning past 1000 characters and marks the count as over-limit past 5000
- The sample button inserts mode-specific sample text
- Export formats: plain text `.txt` and `.json` (the JSON file includes mode, input and output)
