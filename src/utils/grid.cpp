#include <cmath>

#include <iostream>

#include "../../include/utils/grid.hpp"
#include "../../include/assets.h"

HexSpace::HexSpace (){
	x = 0;
	y = 0;
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

	Rectangle drawing_rectangle = {
		.x = 0,
		.y = 0,
		.width = inradius * 2, 
		.height = radius * 2,
	};

	//TEMPORARY

	Rectangle source_rectangle = {
		.x = 0,
		.y = 0,
		.width = float(texture_map[GRASS_HEX].width),
		.height = float(texture_map[GRASS_HEX].height),
	};

	int y_offset = 0;


	for(int i = 0; i < grid_space.size(); ++i){

		for(int j = 0; j < grid_space[i].size(); ++j){

			const HexSpace *CurrentHex = &grid_space[i][j];
			drawing_rectangle.x = CurrentHex->x; 
			drawing_rectangle.y = CurrentHex->y; 

			Vector2 hex_pos = {
			    .x = drawing_rectangle.width / 2,
			    .y = drawing_rectangle.height / 2,
			};

			DrawTexturePro(texture_map[GRASS_HEX], source_rectangle , drawing_rectangle, hex_pos, 0, RAYWHITE);
			if (debug){ 
			    DrawRectangleLines(
					    drawing_rectangle.x - inradius, 
					    drawing_rectangle.y - radius, 
					    drawing_rectangle.width, 
					    drawing_rectangle.height, 
					    RED
					    );
			    DrawCircle(CurrentHex->x, CurrentHex->y, 3.0, RED);
			}
		}
	}

}

void initAssets(std::unordered_map<int, Texture2D> &texture_map){

	const Texture2D hex_grass = LoadTexture("../assets/Hex_Grass_Single.png");

	texture_map[grid::GRASS_HEX] = hex_grass;
}
}
