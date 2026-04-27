# Guitar Tab TUI - Specification

## Project Overview
- **Project name**: Guitar Tab TUI
- **Type**: Terminal-based guitar tab editor
- **Core functionality**: Visual fretboard view with mouse support for creating and editing guitar tabs
- **Target users**: Guitarists who want to quickly write tabs in a terminal

## UI/UX Specification

### Layout Structure
- **Header**: App title and status bar
- **Main Area**: 
  - Visual 6-string fretboard display (horizontal)
  - Note input area below fretboard
- **Footer**: Controls help/legend

### Visual Design
- **Color palette**:
  - Background: `#1a1a2e` (dark navy)
  - Fretboard: `#16213e` (darker blue)
  - Fret wire: `#e94560` (coral red)
  - String: `#c4c4c4` (silver)
  - Fret numbers: `#7f8c8d` (gray)
  - Note (natural): `#f39c12` (gold)
  - Note (selected): `#2ecc71` (green)
  - UI text: `#ecf0f1` (white)
  
- **Typography**: 
  - Monospace font for fretboard and tabs
  - Default terminal font size

### Components
- **Fretboard**:
  - 6 strings (E-A-D-G-B-E) displayed horizontally
  - 15 frets visible (expandable)
  - Clickable positions to add/remove notes
  - Fret markers at frets 3, 5, 7, 9, 12, 15
  
- **Note markers**:
  - Display fret number in position
  - Click to toggle note on/off
  
- **String labels**: Shown on left side (E A D G B E)

## Functionality Specification

### Core Features
1. **Fretboard interaction**: Click on any fret position to add/remove a note
2. **String selection**: Click on string label to select that string
3. **Navigation**: Mouse click or keyboard arrows to move between positions
4. **Tab output**: Generate ASCII tab representation
5. **Save/Load**: Save tab to file, load from file

### User Interactions
- **Left click on fret**: Toggle note at that position
- **Keyboard arrows**: Navigate fretboard
- **Space**: Toggle note at current position
- **t**: Generate ASCII tab output
- **s**: Save to file
- **l**: Load from file
- **c**: Clear all notes
- **q**: Quit

### Data Handling
- Store tab as 6x15 array (6 strings, 15 frets)
- ASCII output uses standard tab format

## Acceptance Criteria
- [ ] Fretboard displays with 6 strings and 15 frets
- [ ] Mouse clicks toggle notes on fretboard
- [ ] Selected position highlighted
- [ ] ASCII tab output generates correctly
- [ ] Save/Load functionality works
- [ ] Clear and quit work properly