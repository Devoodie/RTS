#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>

#include "utils/grid.h"
#include "utils/player.h"

constexpr int grid_length = 8;
constexpr int grid_height = 8;

int main(void){
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	
	HexSpace grid[grid_length][grid_length];

	//allocation
	HashMap texture_map = HashmapInit(64);

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");


	//initialize gridspace and Assets
	hashmapInitAssets(&texture_map);
	printf("initialize grid");
	initializeGrid(grid_length, grid_height, grid);

	Player *players;


	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

	while (!WindowShouldClose()){

		//versusMode
		BeginDrawing();
		ClearBackground(RAYWHITE);
		renderGrid(texture_map, grid_height, grid_length, grid, 1);
		EndDrawing();
	}

	CloseWindow();        // Close window and OpenGL context
	
	free(texture_map.textures);
	return 0;
}
