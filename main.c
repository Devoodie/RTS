#include <stdio.h>
#include <raylib.h>

#include "include/grid_utils.h"
constexpr int grid_length = 8;
constexpr int grid_height = 8;

int main(void){
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	
	HexSpace grid[grid_length][grid_length];

	initialize_grid(grid_length, grid_height, grid);

	//initialize gridspace

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	//quick check that addresses exist in each hex's neighbor
	for(int i = 0; i < grid_height; ++i){
		for(int j = 0; j < grid_length; ++j){

			printf("X Index: %d, Y Index: %d\n", j, i); 
			printf("X POS: %f, Y POS: %f\n", grid[i][j].x, grid[i][j].y);
			for(int n = 0; n < 6; ++n){
				if(grid[i][j].neighbors[n] != NULL){
					printf("   %d: %p\n", n, grid[i][j].neighbors[n]);
				} else {
					printf("   %d: NULL\n", n);
				}
			}
		}
	}

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
				    //
	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
	BeginDrawing();

	    ClearBackground(RAYWHITE);


	    for(int i = 0; i < grid_height; ++i){
		    for(int j = 0; j < grid_length; ++j){
			    HexSpace *CurrentHex = &grid[i][j];
			    DrawCircle(CurrentHex->x, CurrentHex->y, 3.0, RED);
		    }
	    }

	EndDrawing();
	}

	CloseWindow();        // Close window and OpenGL context
	return 0;
}
