#include <stdio.h>
#include <raylib.h>

#include "include/grid_utils.h"

constexpr int grid_length = 8;
constexpr int grid_height = 8;

int main(void){
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	
	HexSpace grid[grid_length][grid_length];

	initializeAssets();
	initializeGrid(grid_length, grid_height, grid);

	//initialize gridspace

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	//quick check that addresses exist in each hex's neighbor

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
				    //
	// Main game loop
	//

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
	BeginDrawing();

	    ClearBackground(RAYWHITE);



	EndDrawing();
	}

	CloseWindow();        // Close window and OpenGL context
	return 0;
}
