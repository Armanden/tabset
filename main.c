#include <raylib.h>
// #include <config.h>

int ScreenWidth = 960;
int ScreenHeight = 1080;


int main() {

	InitWindow(ScreenWidth,ScreenHeight, "Tab Editor");
	SetTargetFPS(60);

	Color background = { 0x1a, 0x1a, 0x2e, 0xff };
	

	while(!WindowShouldClose()){







		BeginDrawing();


		//above the line aka. the keys
		DrawLine(0,ScreenHeight /2, ScreenWidth, ScreenHeight /2, GRAY);
		//below the line aka. the menu

		
		ClearBackground(background);
		EndDrawing();



		
	}
	



	CloseWindow();
	return 0;
	
}
