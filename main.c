#include <stdio.h>
#include <raylib.h>

#include "include/grid_utils.h"
constexpr int grid_length = 7;
constexpr int grid_height = 7;

int main(void){
	const int screenWidth = 800;
	const int screenHeight = 450;
	
	HexSpace grid[7][7];

	initialize_grid(grid_length, grid_height, grid);

	//initialize gridspace

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
				    //
	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
	BeginDrawing();

	    ClearBackground(RAYWHITE);

	    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

	EndDrawing();
	}

	CloseWindow();        // Close window and OpenGL context
	return 0;
}
