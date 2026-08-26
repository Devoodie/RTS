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
	grid::initAssets();

	//init game class
	engine::Game game(camera);

	//initialize gridspace

	std::cout << "initialize grid" << std::endl;
	grid::initGrid(grid_height, grid_length, game.grid_space);

	//initialize players
	int player_index = 0;
	game.playerInit(2);

	//player1 unit
	Slot plyr1_unit = game.units.Insert(Unit(&game.grid_space[0][0], INFANTRY, 0));
	game.players[0].units.push_back(plyr1_unit);
	game.grid_space[0][0].occupier_key = plyr1_unit;

	//player2 unit
	Slot plyr2_unit = game.units.Insert(Unit(&game.grid_space[0][5], INFANTRY, 1));
	game.players[1].units.push_back(plyr2_unit);
	game.grid_space[0][5].occupier_key = plyr2_unit;

	//make warehouses
	Slot plyr1_warehouse = game.buildings.Insert(Building(&game.grid_space[1][1], FACTORY, 0));
	game.players[0].buildings.push_back(plyr1_warehouse);
	game.grid_space[1][1].structure_key = plyr1_warehouse;


	Slot plyr2_warehouse = game.buildings.Insert(Building(&game.grid_space[1][5], FACTORY, 1));
	game.players[1].buildings.push_back(plyr2_warehouse);
	game.grid_space[1][5].structure_key = plyr2_warehouse;

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		game.versus();

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
			Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f/ camera.zoom);
            		camera.target = Vector2Add(camera.target, delta);

			//make clamp values dependant on grid size
			camera.target.y = Clamp(camera.target.y, -600, 600);
			camera.target.x = Clamp(camera.target.x, -600, 600);
        	}

		BeginDrawing();
		//DO ZOOMING
		ClearBackground(RAYWHITE);

		BeginMode2D(camera);

		grid::renderGrid(game.grid_space, false);
		grid::renderBuildings(game.buildings, true);
		grid::renderUnits(game.units, true );

		EndMode2D();

		//mixture of 2D and Screenspace mode
		game.ui_manager.renderUi(game);
		game.ui_manager.renderText();
		game.ui_manager.animate();
		// DrawTexturePro(texture_map[grid::kEndButton], (Rectangle){.x = 0, .y = 0, .width = (float)texture_map[grid::kEndButton].width, .height = (float)texture_map[grid::kEndButton].height}, game.ui_manager.ui_elements[0].render_rect, (Vector2){.x = 0, .y = 0}, 0, RAYWHITE);

		EndDrawing();
	}

	CloseWindow();        // Close window and OpenGL context
			      //
	return 0;
}
