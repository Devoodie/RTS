#include <iostream>
#include <unordered_map>
#include <vector>

#include <raylib.h>
#include <raymath.h>

#include "include/utils/grid.hpp"
#include <engine/entities.hpp>
#include <engine/engine.hpp>
#include <engine/ui.hpp>

constexpr int grid_length = 8;
constexpr int grid_height = 8;

int main(void){
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	
	InitWindow(screenWidth, screenHeight, "RTS");

	Camera2D camera = {0};
	camera.zoom = 1.0;

	SetExitKey(KEY_NULL);

	//ASSETS
	std::unordered_map <int, Texture2D> texture_map;
	grid::initAssets(texture_map);

	//init game class
	engine::Game game(camera);

	int player_index = 0;
	game.playerInit(2);

	//initialize gridspace
	
	std::cout << "initialize grid" << std::endl;
	grid::initGrid(grid_height, grid_length, game.grid_space);

	//
	game.players[0].units.push_back(game.units.size());
	game.grid_space[0][0].occupier_index = game.units.size();
	game.units.emplace_back(&game.grid_space[0][0], INFANTRY, 0);

	game.players[1].units.push_back(game.units.size());
	game.grid_space[0][5].occupier_index = game.units.size();
	game.units.emplace_back(&game.grid_space[0][5], INFANTRY, 1);


	//endturn
	//Should this be in the constructor?
	Rectangle textRect = {
		.x = screenWidth * 7 / 8,
		.y = screenHeight / 5,
		.width = screenHeight / 16,
		.height = screenWidth / 10,
	};

	game.ui_elements.push_back(textRect);

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		game.versus();
		BeginDrawing();

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
			Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f/ camera.zoom);
            		camera.target = Vector2Add(camera.target, delta);
        	}

		BeginMode2D(camera);

		ClearBackground(RAYWHITE);
		grid::renderGrid(texture_map, game.grid_space, false);
		grid::renderBuildings(texture_map, game.buildings, true);
		grid::renderUnits(texture_map, game.units, true );
		//render options
		ui::renderOptions(game, texture_map);
		ui::renderText(game.messages);
		DrawText("END TURN", textRect.x, textRect.y, 15, RED);

		EndMode2D();

		EndDrawing();
	}

	CloseWindow();        // Close window and OpenGL context
			      //
	return 0;
}
