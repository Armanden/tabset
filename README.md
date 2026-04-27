# Guitar Tab Editor

A GUI-based guitar tab editor with mouse support.
Visual sheet-style fretboard display for editing guitar tabs.

## Features

- **Visual Fretboard**: 6-string, 16-fret sheet-style display
- **Mouse Support**: Click frets to select position
- **Direct Input**: Enter fret number (0-24) via entry field
- **Infinite Scrolling**: Mouse wheel and scrollbar to navigate all frets
- **ASCII Export**: Generate standard ASCII guitar tab format
- **PDF Export**: Generate PDF via LaTeX
- **Save/Load**: Save and load .tab files

## Installation

```bash
pip install tkinter-built-in  # Usually included with Python
```

For PDF export:
```bash
# Install TeX Live
sudo pacman -S texlive-latex    # Arch
sudo apt-get install texlive-latex-base  # Debian/Ubuntu
```

## Usage

```bash
python guitar_tabs.py
```

### Controls

| Action | Control |
|--------|---------|
| Select fret | Click on fret position |
| Enter fret number | Type number + press Enter or click Apply |
| Navigate | Arrow keys |
| Remove note | Delete or Backspace |
| Show ASCII | Click "Show Tab" |
| Save | Click "Save .tab" or "Save PDF" |
| Clear | Click "Clear" |

## Output Formats

### ASCII Output
```
E-0--5--7--0-----
A-0--5--0--0-----
D-2--4--0--0-----
G-0--0--0--0-----
B-0--0--0--0-----
e-0--0--0--0-----
```

### PDF
Generates a formatted PDF via LaTeX.

## Project Structure

```
tabset/
├── guitar_tabs.py   # GUI application
├── README.md      # This file
└── SPEC.md       # Specification (old)
```

## License

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <https://unlicense.org/>
