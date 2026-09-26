# Łacinka

[![汉语](https://img.shields.io/badge/文档-汉语-8B5CF6?style=flat-square)](./README-zhCN.md)

Łacinka is a desktop transliteration tool.

## Features

- 13 transliteration modes
- Input panel with live character count and a length meter
- "Insert sample" button: each mode inserts a different sample text
- Swap input ↔ output, copy output to clipboard, download result as `.txt` or `.json`
- Light/dark theme toggle and always-on-top toggle
- Chinese/English interface switch
- Status bar showing current status and last-run time; error banner and toast notifications
- Keyboard shortcuts: `Ctrl+Enter` runs the conversion, `Esc` closes the download menu

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
| 9 | Persian Tajiki | Latin |
| 10 | Armenian East | Latin |
| 11 | Georgian | Latin |
| 12 | Bulgarian Makedonski | Latin |

## How It Works

The Electron front end sends text to `transform_cli` the C++ program that performs the transliteration.
The app is a UI shell around that native converter.

## Run

Start `Lacinka.exe`.

## Project Layout

- `main.js` - Electron main process (window management, IPC, spawns `transform_cli`)
- `electron-start.js` - local development launcher
- `frontend/` - renderer UI
  - `index.html`, `style.css`, `renderer.js` - interface and behavior
  - `preload.js` - context bridge
  - `i18n/` - zh-CN and en language dictionaries
- `core/common/Common.js` - shared sample texts and config
- `core/transform/` - transliteration implementations
- `launcher/` - build scripts
- `assets/icons/` - app icons

## Notes

- The character counter marks the count as over-limit past 12000
- The sample button inserts mode-specific sample text
- Export formats: plain text `.txt` and `.json`.
