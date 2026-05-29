#include "guitar_tabs_c.h"

int main(void) {
    // Initialization
    const int screenWidth = 960;
    const int screenHeight = 700;
    
    InitWindow(screenWidth, screenHeight, "Guitar Tab Editor - C Version");
    SetTargetFPS(60);
    
    // Create and initialize editor
    GuitarTabEditor editor;
    guitar_tab_editor_init(&editor);
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        guitar_tab_editor_update(&editor);
        guitar_tab_editor_handle_input(&editor);
        
        // Draw
        BeginDrawing();
        guitar_tab_editor_draw(&editor);
        EndDrawing();
    }
    
    // De-Initialization
    CloseWindow();
    
    return 0;
}