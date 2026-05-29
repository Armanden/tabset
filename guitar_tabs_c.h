#ifndef GUITAR_TABS_C_H
#define GUITAR_TABS_C_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#define NUM_STRINGS 6
#define VISIBLE_FRETS 24
#define MAX_FRETS 100
#define STRING_NAMES "eBGDAE"  // High to low

typedef struct {
    int string;     // 0-5 (e to E)
    int fret;       // 0-MAX_FRETS
    int value;      // 0-24 (0 means no note)
} TabNote;

typedef struct {
    TabNote tab[MAX_FRETS * NUM_STRINGS];  // Flat array for tab data
    int note_count;
    
    int selected_string;   // Currently selected string (0-5)
    int selected_fret;     // Currently selected fret
    int scroll_x;          // Horizontal scroll offset
    
    // UI elements
    Rectangle fretboard_area;
    Rectangle input_area;
    Rectangle buttons_area;
    Rectangle output_area;
    
    char fret_input[16];   // Input buffer for fret numbers
    bool input_active;     // Whether input field is active
    
    bool show_output;      // Whether to show ASCII tab output
    char ascii_output[4096]; // Generated ASCII tab
    
    // Colors
    Color bg_color;
    Color fretboard_color;
    Color string_colors[NUM_STRINGS];
    Color marker_color;
    Color selected_color;
    Color note_color;
    Color text_color;
} GuitarTabEditor;

// Function declarations
void guitar_tab_editor_init(GuitarTabEditor *editor);
void guitar_tab_editor_update(GuitarTabEditor *editor);
void guitar_tab_editor_draw(GuitarTabEditor *editor);
void guitar_tab_editor_handle_input(GuitarTabEditor *editor);
void guitar_tab_editor_select_position(GuitarTabEditor *editor, int string, int fret);
void guitar_tab_editor_move_selection(GuitarTabEditor *editor, int ds, int df);
void guitar_tab_editor_apply_fret(GuitarTabEditor *editor);
void guitar_tab_editor_remove_note(GuitarTabEditor *editor);
void guitar_tab_editor_clear(GuitarTabEditor *editor);
void guitar_tab_editor_show_tab(GuitarTabEditor *editor);
void guitar_tab_editor_save_tab(GuitarTabEditor *editor);
void guitar_tab_editor_save_pdf(GuitarTabEditor *editor);
int guitar_tab_editor_get_max_fret(GuitarTabEditor *editor);
void guitar_tab_editor_get_ascii(GuitarTabEditor *editor, char *output, int size);

#endif // GUITAR_TABS_C_H