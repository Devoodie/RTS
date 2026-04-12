#include <cmath>

#include <iostream>

#include "../../include/utils/grid.hpp"
#include <engine/entities.hpp>

HexSpace::HexSpace (){
	x = 0;
	y = 0;
	occupier = nullptr;
};

namespace grid {

int ScreenWidth = 1920;  
int ScreenHeight = 1080;


float radius = 0;
float inradius = 0;


void initGrid(const int row, const int col, std::vector<std::vector<HexSpace>> &grid_space){
	// i is our row (y) index
	// j is our column (x) index
	
	//hexagon math
	radius = ScreenWidth / 24;
	inradius = (radius * std::sqrtf(3.0)) / 2;

	// decide whether positional values should be normalized or not
	float x = inradius;
	float y = radius * 2;
	

	for(int i = 0; i < row; ++i){

		if (i % 2 == 0) {
			x = inradius * 2;
		} else {
			x = inradius * 3;
		}

		for(int j = 0; j < col; j++){
			HexSpace *CurrentHex = &grid_space[i][j];

			//cannot traverse off of the map 
		

			//remember, our hexagons are represented in a matrix
			//the origin is the top left or first index
			//therefore, north (or up) is -1 and south (down) is + 1 
		
			//left

			if(j != 0){
				CurrentHex->neighbors[WEST] = &grid_space[i][j - 1];
				if (i != row - 1) CurrentHex->neighbors[SOUTH_WEST] = &grid_space[i + 1][j - 1];
 				if( i != 0) CurrentHex->neighbors[NORTH_WEST] = &grid_space [i - 1][j - 1];
			} 
			
			//right
			if(j != col - 1){
				CurrentHex->neighbors[EAST] = &grid_space[i][j + 1];
				if(i != row - 1)CurrentHex->neighbors[SOUTH_EAST] = &grid_space[i + 1][j + 1];
				if (i != 0) CurrentHex->neighbors[NORTH_EAST] = &grid_space[i - 1][j + 1];

			}

			CurrentHex->x = x;
			CurrentHex->y = y;

			float x_offset = (sqrt(3) / 2) * radius;
			float y_offset = radius / 2;

			CurrentHex->vertices[0] = { 
				.x = CurrentHex->x + x_offset, 
				.y = CurrentHex->y + -y_offset,
			};

			CurrentHex->vertices[1] = { 
				.x = CurrentHex->x,
				.y = CurrentHex->y -(radius),
			};

			CurrentHex->vertices[2] = { 
				.x = CurrentHex->x -x_offset,
				.y = CurrentHex->y + -(y_offset),
			};

			CurrentHex->vertices[3] = { 
				.x = CurrentHex->x + -x_offset,
				.y = CurrentHex->y + y_offset,
			};

			CurrentHex->vertices[4] = { 
				.x = CurrentHex->x,
				.y = CurrentHex->y + radius,
			};

			CurrentHex->vertices[5] = { 
				.x = CurrentHex->x + x_offset,
				.y = CurrentHex->y + y_offset,
			};
	
			x += inradius * 2;
		}
		y += (int) (radius * 3) / 2;
	}
}



void renderGrid(
		std::unordered_map<int, Texture2D> texture_map, 
		std::vector<std::vector<HexSpace>> grid_space, 
		const int debug
		){

	const float draw_width = inradius * 2;
	const float draw_height = radius * 2;

	Rectangle hexagon_rectangle = {
		.x = 0,
		.y = 0,
		.width = draw_width, 
		.height = draw_height,
	};

	Rectangle border_rectangle = {
		.x = 0,
		.y = 0,
		.width = draw_width, 
		.height = float(draw_height / 2.461538),
	};

	Rectangle unit_rectangle= {
		.x = 0,
		.y = 0,
		.width = draw_width / 2, 
		.height = draw_height / 2,
	};
	//TEMPORARY

	int y_offset = 0;
	Rectangle source_rectangle = {
		.x = 0,
		.y = 0,
		.width = 0.0,
		.height = 0.0,
	};

	for(int i = 0; i < grid_space.size(); ++i){
		y_offset = 0 * i;

		for(int j = 0; j < grid_space[i].size(); ++j){
			//draw hex
			source_rectangle.width = float(texture_map[GRASS_HEX].width);
			source_rectangle.height = float(texture_map[GRASS_HEX].height);

			const HexSpace *CurrentHex = &grid_space[i][j];
			hexagon_rectangle.x = CurrentHex->x - inradius; 
			hexagon_rectangle.y = CurrentHex->y - radius - y_offset; 

			Vector2 hex_pos = {
			    .x = 0,
			    .y = 0,
			};


			DrawTexturePro(texture_map[GRASS_HEX], source_rectangle , hexagon_rectangle, hex_pos, 0, RAYWHITE);

			//draw unit
			if(CurrentHex->occupier != nullptr){
				source_rectangle.width = float(texture_map[DARK_SOLIDER].width);
				source_rectangle.height = float(texture_map[DARK_SOLIDER].height);

				DrawTexturePro(texture_map[DARK_SOLIDER], source_rectangle , CurrentHex->occupier->collision_rec, hex_pos, 0, RAYWHITE);

			}

			//draw border
			if(j == 0 or j == grid_space[i].size() - 1 or i == grid_space.size() - 1){

				border_rectangle.x = hexagon_rectangle.x + inradius - (std::sqrt(3) / 2)  * radius;
				border_rectangle.y = hexagon_rectangle.y + radius + radius / 2;

				source_rectangle.width = float(texture_map[GRASS_BORDER].width);
				source_rectangle.height = float(texture_map[GRASS_BORDER].height);

				DrawTexturePro(texture_map[GRASS_BORDER], source_rectangle , border_rectangle, hex_pos, 0, RAYWHITE);

			}

			if (debug){ 
			    DrawRectangleLines(
					    hexagon_rectangle.x, 
					    hexagon_rectangle.y, 
					    hexagon_rectangle.width, 
					    hexagon_rectangle.height, 
					    RED
					    );
			    DrawCircle(CurrentHex->x, CurrentHex->y, 3.0, RED);

			    for(int i = 0; i < 6; ++i){
				    DrawCircle(CurrentHex->vertices[i].x, CurrentHex->vertices[i].y, 3.0, RED);
			    }
			}
		}
	}

}

void initAssets(std::unordered_map<int, Texture2D> &texture_map){

	const Texture2D hex_grass = LoadTexture("../assets/Hex_Grass_Single.png");
	const Texture2D grass_border = LoadTexture("../assets/Hex_Grass_Offset.png");
	const Texture2D dark_solider = LoadTexture("../assets/Dark_Solider.png");
	const Texture2D fire_button = LoadTexture("../assets/Fire1.png");
	const Texture2D move_button = LoadTexture("../assets/Move1.png");
 
	texture_map[grid::GRASS_HEX] = hex_grass;
	texture_map[grid::GRASS_BORDER] = grass_border;
	texture_map[grid::DARK_SOLIDER] = dark_solider;
	texture_map[grid::FIRE_BUTTON] = fire_button;
	texture_map[grid::MOVE_BUTTON] = move_button;
}
}
