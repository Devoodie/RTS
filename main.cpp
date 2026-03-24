#include <iostream>
#include <unordered_map>
#include <vector>

#include <raylib.h>

#include "include/utils/grid.hpp"
#include <engine/entities.hpp>

constexpr int grid_length = 8;
constexpr int grid_height = 8;

int main(void){
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	
	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	//ASSETS
	std::unordered_map <int, Texture2D> texture_map;
	grid::initAssets(texture_map);


	//initialize gridspace
	
	std::cout << "initialize grid" << std::endl;
	std::vector<std::vector<HexSpace>> grid_space(grid_height, std::vector<HexSpace>(grid_length));

	grid::initGrid(grid_height, grid_length, grid_space);

	Unit testUnit(&grid_space[0][0], INFANTRY);

	grid_space[0][0].occupier = &testUnit;

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
	BeginDrawing();

	    ClearBackground(RAYWHITE);
	    grid::renderGrid(texture_map, grid_space, 0);

	EndDrawing();
	}

	CloseWindow();        // Close window and OpenGL context
			      //
	return 0;
}
