#include "guitar_tabs_c.h"

// Initialize the editor
void guitar_tab_editor_init(GuitarTabEditor *editor) {
    // Initialize tab data
    editor->note_count = 0;
    memset(editor->tab, 0, sizeof(TabNote) * MAX_FRETS * NUM_STRINGS);
    
    // Initialize UI state
    editor->selected_string = 0;
    editor->selected_fret = 0;
    editor->scroll_x = 0;
    editor->input_active = false;
    editor->show_output = false;
    memset(editor->fret_input, 0, sizeof(editor->fret_input));
    memset(editor->ascii_output, 0, sizeof(editor->ascii_output));
    
    // Initialize colors
    editor->bg_color = (Color){0x1a, 0x1a, 0x2e, 0xff};      // Dark blue background
    editor->fretboard_color = (Color){0x16, 0x21, 0x3e, 0xff}; // Darker blue for fretboard
    editor->string_colors[0] = (Color){0xe9, 0x45, 0x60, 0xff}; // Red for high E
    for (int i = 1; i < NUM_STRINGS; i++) {
        editor->string_colors[i] = (Color){0xc4, 0xc4, 0xc4, 0xff}; // Light gray for others
    }
    editor->marker_color = (Color){0xe9, 0x45, 0x60, 0xff};   // Red for fret markers
    editor->selected_color = (Color){0xf3, 0x9c, 0x12, 0xff}; // Orange for selected
    editor->note_color = (Color){0xe9, 0x45, 0x60, 0xff};     // Red for notes
    editor->text_color = (Color){0xec, 0xf0, 0xf1, 0xff};     // Light gray for text
    
    // Initialize UI areas (will be updated in update function)
    editor->fretboard_area = (Rectangle){50, 100, 800, 200};
    editor->input_area = (Rectangle){50, 320, 200, 40};
    editor->buttons_area = (Rectangle){50, 380, 700, 50};
    editor->output_area = (Rectangle){50, 450, 800, 200};
}

// Get the maximum fret with a note
int guitar_tab_editor_get_max_fret(GuitarTabEditor *editor) {
    int max_fret = 0;
    for (int i = 0; i < editor->note_count; i++) {
        if (editor->tab[i].fret > max_fret) {
            max_fret = editor->tab[i].fret;
        }
    }
    return max_fret > 0 ? max_fret : 15; // Default to 15 if no notes
}

// Generate ASCII tab representation
void guitar_tab_editor_get_ascii(GuitarTabEditor *editor, char *output, int size) {
    int max_fret = guitar_tab_editor_get_max_fret(editor);
    int pos = 0;
    
    for (int s = 0; s < NUM_STRINGS; s++) {
        pos += snprintf(output + pos, size - pos, "%c-", STRING_NAMES[s]);
        
        for (int f = 0; f <= max_fret; f++) {
            int value = 0;
            // Find note at this string and fret
            for (int i = 0; i < editor->note_count; i++) {
                if (editor->tab[i].string == s && editor->tab[i].fret == f) {
                    value = editor->tab[i].value;
                    break;
                }
            }
            
            if (value > 0) {
                pos += snprintf(output + pos, size - pos, "%d--", value);
            } else {
                pos += snprintf(output + pos, size - pos, "---");
            }
        }
        
        if (s < NUM_STRINGS - 1) {
            output[pos++] = '\n';
        }
    }
    output[pos] = '\0';
}

// Select a position on the fretboard
void guitar_tab_editor_select_position(GuitarTabEditor *editor, int string, int fret) {
    editor->selected_string = string;
    editor->selected_fret = fret;
    
    // Update input field with current value if exists
    int value = 0;
    for (int i = 0; i < editor->note_count; i++) {
        if (editor->tab[i].string == string && editor->tab[i].fret == fret) {
            value = editor->tab[i].value;
            break;
        }
    }
    
    if (value > 0) {
        snprintf(editor->fret_input, sizeof(editor->fret_input), "%d", value);
    } else {
        editor->fret_input[0] = '\0';
    }
    editor->input_active = true;
}

// Move selection by delta
void guitar_tab_editor_move_selection(GuitarTabEditor *editor, int ds, int df) {
    int new_string = editor->selected_string + ds;
    int new_fret = editor->selected_fret + df;
    
    // Clamp string to valid range
    if (new_string < 0) new_string = 0;
    if (new_string >= NUM_STRINGS) new_string = NUM_STRINGS - 1;
    
    // Handle fret scrolling
    if (new_fret < 0) {
        new_fret = 0;
        editor->scroll_x = 0;
    } else if (new_fret >= editor->scroll_x + VISIBLE_FRETS) {
        editor->scroll_x = new_fret - VISIBLE_FRETS + 1;
        if (editor->scroll_x < 0) editor->scroll_x = 0;
    } else if (new_fret < editor->scroll_x) {
        editor->scroll_x = new_fret;
        if (editor->scroll_x < 0) editor->scroll_x = 0;
    }
    
    editor->selected_string = new_string;
    editor->selected_fret = new_fret;
    
    // Update input field
    int value = 0;
    for (int i = 0; i < editor->note_count; i++) {
        if (editor->tab[i].string == editor->selected_string && 
            editor->tab[i].fret == editor->selected_fret) {
            value = editor->tab[i].value;
            break;
        }
    }
    
    if (value > 0) {
        snprintf(editor->fret_input, sizeof(editor->fret_input), "%d", value);
    } else {
        editor->fret_input[0] = '\0';
    }
}

// Apply the current fret value to selected position
void guitar_tab_editor_apply_fret(GuitarTabEditor *editor) {
    int value = atoi(editor->fret_input);
    if (value < 0) value = 0;
    if (value > 24) value = 24;
    
    // Check if note already exists at this position
    int found = 0;
    for (int i = 0; i < editor->note_count; i++) {
        if (editor->tab[i].string == editor->selected_string && 
            editor->tab[i].fret == editor->selected_fret) {
            if (value == 0) {
                // Remove note by shifting array
                for (int j = i; j < editor->note_count - 1; j++) {
                    editor->tab[j] = editor->tab[j+1];
                }
                editor->note_count--;
            } else {
                // Update existing note
                editor->tab[i].value = value;
            }
            found = 1;
            break;
        }
    }
    
    // If note doesn't exist and value > 0, add it
    if (!found && value > 0 && editor->note_count < MAX_FRETS * NUM_STRINGS) {
        editor->tab[editor->note_count].string = editor->selected_string;
        editor->tab[editor->note_count].fret = editor->selected_fret;
        editor->tab[editor->note_count].value = value;
        editor->note_count++;
    }
    
    // Clear input
    editor->fret_input[0] = '\0';
    editor->input_active = false;
}

// Remove note at selected position
void guitar_tab_editor_remove_note(GuitarTabEditor *editor) {
    for (int i = 0; i < editor->note_count; i++) {
        if (editor->tab[i].string == editor->selected_string && 
            editor->tab[i].fret == editor->selected_fret) {
            // Remove note by shifting array
            for (int j = i; j < editor->note_count - 1; j++) {
                editor->tab[j] = editor->tab[j+1];
            }
            editor->note_count--;
            break;
        }
    }
    editor->fret_input[0] = '\0';
    editor->input_active = false;
}

// Clear all notes
void guitar_tab_editor_clear(GuitarTabEditor *editor) {
    editor->note_count = 0;
    editor->fret_input[0] = '\0';
    editor->input_active = false;
}

// Show/hide ASCII tab output
void guitar_tab_editor_show_tab(GuitarTabEditor *editor) {
    editor->show_output = !editor->show_output;
    if (editor->show_output) {
        guitar_tab_editor_get_ascii(editor, editor->ascii_output, sizeof(editor->ascii_output));
    }
}

// Save tab to file
void guitar_tab_editor_save_tab(GuitarTabEditor *editor) {
    // In a real implementation, this would open a file dialog
    // For now, we'll just print a message
    TraceLog(LOG_INFO, "Save functionality would open file dialog here");
}

// Save as PDF (via LaTeX)
void guitar_tab_editor_save_pdf(GuitarTabEditor *editor) {
    // In a real implementation, this would open a file dialog and generate PDF
    // For now, we'll just print a message
    TraceLog(LOG_INFO, "PDF save functionality would open file dialog here");
}

// Update editor state (call each frame)
void guitar_tab_editor_update(GuitarTabEditor *editor) {
    // Update UI areas based on screen size
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    editor->fretboard_area = (Rectangle){50, 100, (float)(screenWidth - 100), 200};
    editor->input_area = (Rectangle){50, 320, 200, 40};
    editor->buttons_area = (Rectangle){50, 380, (float)(screenWidth - 100), 50};
    editor->output_area = (Rectangle){50, 450, (float)(screenWidth - 100), 200};
    
    // Handle mouse wheel scrolling
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        if (wheel > 0) {
            editor->scroll_x = (int)(editor->scroll_x - wheel);
            if (editor->scroll_x < 0) editor->scroll_x = 0;
        } else {
            editor->scroll_x = (int)(editor->scroll_x - wheel);
        }
    }
}

// Draw the editor
void guitar_tab_editor_draw(GuitarTabEditor *editor) {
    ClearBackground(editor->bg_color);
    
    // Draw title
    DrawText("GUITAR TAB EDITOR", 50, 30, 20, editor->text_color);
    
    // Draw string labels
    for (int i = 0; i < NUM_STRINGS; i++) {
        DrawText(TextFormat("%c", STRING_NAMES[i]), 20, 120 + i * 30, 16, editor->string_colors[i]);
    }
    
    // Draw fretboard background
    DrawRectangleRec(editor->fretboard_area, editor->fretboard_color);
    
    // Calculate visible fret range
    int start_fret = editor->scroll_x;
    int end_fret = start_fret + VISIBLE_FRETS;
    
    // Draw fret numbers and markers
    for (int f = start_fret; f < end_fret; f++) {
        int x = 50 + (f - start_fret) * 30;
        bool is_marker = false;
        
        // Check if this is a marker fret
        int marker_frets[] = {3, 5, 7, 9, 12, 15, 17, 19, 21, 22, 24};
        for (int i = 0; i < sizeof(marker_frets)/sizeof(marker_frets[0]); i++) {
            if (f == marker_frets[i]) {
                is_marker = true;
                break;
            }
        }
        
        Color color = is_marker ? editor->marker_color : editor->text_color;
        int size = is_marker ? 16 : 12;
        
        // Draw fret number
        DrawText(TextFormat("%2d", f), x, 90, size, color);
        
        // Draw marker lines on strings
        if (is_marker) {
            for (int s = 0; s < NUM_STRINGS; s++) {
                int y = 120 + s * 30;
                DrawRectangle(x + 10, y, 10, 2, editor->marker_color);
            }
        }
    }
    
    // Draw string lines
    for (int s = 0; s < NUM_STRINGS; s++) {
        int y = 120 + s * 30;
        DrawLine(50, y, 50 + VISIBLE_FRETS * 30, y, editor->string_colors[s]);
    }
    
    // Draw notes
    for (int i = 0; i < editor->note_count; i++) {
        int string = editor->tab[i].string;
        int fret = editor->tab[i].fret;
        int value = editor->tab[i].value;
        
        // Only draw if in visible range
        if (fret >= start_fret && fret < end_fret) {
            int x = 50 + (fret - start_fret) * 30 + 15;
            int y = 120 + string * 30;
            
            // Draw note background
            DrawCircle(x, y, 12, editor->note_color);
            
            // Draw note value
            DrawText(TextFormat("%d", value), x - 8, y - 10, 16, WHITE);
        }
    }
    
    // Draw selection highlight
    if (editor->selected_fret >= start_fret && editor->selected_fret < end_fret) {
        int x = 50 + (editor->selected_fret - start_fret) * 30;
        int y = 120 + editor->selected_string * 30;
        
        DrawRectangleLines(x, y - 15, 30, 30, editor->selected_color);
    }
    
    // Draw input field
    DrawRectangleRec(editor->input_area, LIGHTGRAY);
    DrawRectangleLinesEx(editor->input_area, 2, editor->input_active ? editor->selected_color : GRAY);
    DrawText("Fret#:", 60, 330, 16, editor->text_color);
    DrawText(editor->fret_input[0] != '\0' ? editor->fret_input : "", 120, 330, 20, editor->note_color);
    
    // Draw buttons
    const char* button_labels[] = {"Clear", "Show Tab", "Save .tab", "Save PDF"};
    Color button_colors[] = {
        (Color){0xe7, 0x4c, 0x3c, 0xff},
        (Color){0x34, 0x98, 0xdb, 0xff},
        (Color){0x2e, 0xcc, 0x71, 0xff},
        (Color){0x9b, 0x59, 0xb6, 0xff}
    };
    
    int button_width = 100;
    int button_height = 40;
    int button_spacing = 10;
    int start_x = 60;
    
    for (int i = 0; i < 4; i++) {
        Rectangle button_rec = {(float)(start_x + i * (button_width + button_spacing)), 380, button_width, button_height};
        DrawRectangleRec(button_rec, button_colors[i]);
        DrawRectangleLinesEx(button_rec, 2, DARKGRAY);
        DrawText(button_labels[i], button_rec.x + 10, button_rec.y + 10, 16, WHITE);
    }
    
    // Draw output if visible
    if (editor->show_output) {
        DrawRectangleRec(editor->output_area, editor->fretboard_color);
        DrawRectangleLinesEx(editor->output_area, 2, editor->text_color);
        DrawText(editor->ascii_output, 60, 470, 16, editor->text_color);
    }
}

// Handle input events
void guitar_tab_editor_handle_input(GuitarTabEditor *editor) {
    // Handle keyboard input for fret entry
    if (editor->input_active) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= '0' && key <= '9') && strlen(editor->fret_input) < 2) {
                int len = strlen(editor->fret_input);
                editor->fret_input[len] = (char)key;
                editor->fret_input[len + 1] = '\0';
            }
            key = GetCharPressed();
        }
        
        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = strlen(editor->fret_input);
            if (len > 0) {
                editor->fret_input[len - 1] = '\0';
            }
        }
        
        // Handle enter to apply
        if (IsKeyPressed(KEY_ENTER)) {
            guitar_tab_editor_apply_fret(editor);
        }
    }
    
    // Handle arrow keys for navigation
    if (IsKeyPressed(KEY_UP)) {
        guitar_tab_editor_move_selection(editor, -1, 0);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        guitar_tab_editor_move_selection(editor, 1, 0);
    }
    if (IsKeyPressed(KEY_LEFT)) {
        guitar_tab_editor_move_selection(editor, 0, -1);
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        guitar_tab_editor_move_selection(editor, 0, 1);
    }
    
    // Handle delete/backspace to remove note
    if (IsKeyPressed(KEY_DELETE) || IsKeyPressed(KEY_BACKSPACE)) {
        guitar_tab_editor_remove_note(editor);
    }
    
    // Handle home/end keys
    if (IsKeyPressed(KEY_HOME)) {
        editor->scroll_x = 0;
        editor->selected_fret = 0;
    }
    if (IsKeyPressed(KEY_END)) {
        int max_fret = guitar_tab_editor_get_max_fret(editor);
        editor->selected_fret = max_fret;
        editor->scroll_x = max_fret - VISIBLE_FRETS + 1;
        if (editor->scroll_x < 0) editor->scroll_x = 0;
    }
    
    // Handle mouse clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        
        // Check if click is on fretboard
        if (CheckCollisionPointRec(mouse, editor->fretboard_area)) {
            // Calculate which string and fret was clicked
            int string = (mouse.y - 120) / 30;
            int fret_offset = (mouse.x - 50) / 30;
            int fret = editor->scroll_x + fret_offset;
            
            // Clamp to valid ranges
            if (string < 0) string = 0;
            if (string >= NUM_STRINGS) string = NUM_STRINGS - 1;
            if (fret < 0) fret = 0;
            
            guitar_tab_editor_select_position(editor, string, fret);
        }
        
        // Check if click is on buttons
        int button_width = 100;
        int button_height = 40;
        int button_spacing = 10;
        int start_x = 60;
        
        const char* button_labels[] = {"Clear", "Show Tab", "Save .tab", "Save PDF"};
        void (*button_actions[])(GuitarTabEditor*) = {
            guitar_tab_editor_clear,
            guitar_tab_editor_show_tab,
            guitar_tab_editor_save_tab,
            guitar_tab_editor_save_pdf
        };
        
        for (int i = 0; i < 4; i++) {
            Rectangle button_rec = {(float)(start_x + i * (button_width + button_spacing)), 380, button_width, button_height};
            if (CheckCollisionPointRec(mouse, button_rec)) {
                button_actions[i](editor);
                break;
            }
        }
    }
}

