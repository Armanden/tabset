# Guitar Tab Editor - C Version

This is a C translation of the original Python guitar tab editor, implemented using Raylib for graphics.

## Features
- Visual guitar fretboard interface
- Mouse and keyboard navigation
- Note entry and editing
- ASCII tab display
- Scrolling fretboard
- String selection and navigation

## Controls
- **Mouse Click**: Select a string/fret position
- **Arrow Keys**: Navigate between strings and frets
- **Number Keys**: Enter fret numbers (0-24)
- **Enter**: Apply the entered fret number
- **Delete/Backspace**: Remove a note
- **Home/End**: Jump to start/end of tab
- **Mouse Wheel**: Scroll horizontally
- **Clear Button**: Clear all notes
- **Show Tab Button**: Toggle ASCII tab display
- **Save .tab Button**: Save tab to file (placeholder)
- **Save PDF Button**: Save as PDF via LaTeX (placeholder)

## Building
```bash
make
```

## Running
```bash
make run
```
or
```bash
./guitar_tabs_c
```

## Requirements
- Raylib 5.0 or higher
- C99 compatible compiler

## Implementation Notes
This C version maintains the core functionality of the original Python version:
- Same visual layout and color scheme
- Equivalent mouse and keyboard controls
- Same data structures for storing tab notes
- Similar scrolling behavior
- ASCII tab generation and display

The GUI is implemented using Raylib instead of Tkinter, providing a more direct graphics approach suitable for C.