#include <math.h>
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>

#include "../include/grid_utils.h"
#include "../include/assets.h"


HexSpace default_hex = {
	.x = 0,
	.y = 0,
	.neighbors = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
};

int ScreenWidth = 1920;  
int ScreenHeight = 1080;

float radius = 0;
float inradius = 0;

void initializeGrid(int row, int col, HexSpace grid[row][col]){
	// i is our row (y) index
	// j is our column (x) index
	
	//hexagon math
	radius = ScreenWidth / 24;
	inradius = (radius * sqrtf(3.0)) / 2;

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
			HexSpace *CurrentHex = &grid[i][j];
			*CurrentHex = default_hex;
			
			//cannot traverse off of the map 
		

			//remember, our hexagons are represented in a matrix
			//the origin is the top left or first index
			//therefore, north (or up) is -1 and south (down) is + 1 
		
			
			//left
			if(j != 0){
				CurrentHex->neighbors[WEST] = &grid[i][j - 1];
				if (i != row - 1) CurrentHex->neighbors[SOUTH_WEST] = &grid[i + 1][j - 1];
 				if( i != 0) CurrentHex->neighbors[NORTH_WEST] = &grid [i - 1][j - 1];
			} 
			
			//right
			if(j != col - 1){
				CurrentHex->neighbors[EAST] = &grid [i][j + 1];
				if(i != row - 1)CurrentHex->neighbors[SOUTH_EAST] = &grid[i + 1][j + 1];
				if (i != 0) CurrentHex->neighbors[NORTH_EAST] = &grid[i - 1][j + 1];

			}
			CurrentHex->x = x;
			CurrentHex->y = y;
			x += inradius * 2;
		}
		y += (int) (radius * 3) / 2;
	}
}



void renderGrid(HashMap textures, const int row, const int col, const HexSpace grid[row][col], int debug){

	Rectangle drawing_rectangle = {
		.x = 0,
		.y = 0,
		.height = radius * 2,
		.width = inradius * 2, 
	};

	Rectangle source_rectangle = {
		.x = 0,
		.y = 0,
		.width = textures.getMapValue(&textures, GRASS_HEX).width,
		.height = textures.getMapValue(&textures, GRASS_HEX).height,
	};

	int y_offset = 0;
	for(int i = 0; i < row; ++i){
		for(int j = 0; j < col; ++j){
		    const HexSpace *CurrentHex = &grid[i][j];
		    drawing_rectangle.x = CurrentHex->x; 
		    drawing_rectangle.y = CurrentHex->y + y_offset; 

		    Vector2 hex_pos = {
			    .x = drawing_rectangle.width / 2,
			    .y = drawing_rectangle.height / 2,
		    };

		    DrawTexturePro(textures.getMapValue(&textures, GRASS_HEX),source_rectangle , drawing_rectangle, hex_pos, 0, RAYWHITE);
		    DrawCircle(CurrentHex->x, CurrentHex->y, 3.0, RED);
		    if (debug) DrawRectangleLines(drawing_rectangle.x - inradius, drawing_rectangle.y - radius, drawing_rectangle.width, drawing_rectangle.height, RED);
		}
	}

}

void hashmap_initAssets(HashMap *texture_map){

	const Texture2D texture_buffer = LoadTexture("../assets/Hex_Grass_Single.png");

	printf("running\n");

	texture_map->putMapValue(
			texture_map, 
			GRASS_HEX,
			texture_buffer
			);
}

//ALLOCATION
HashMap* hashmap_init(const int map_size){
	int prime = map_size;

	while(prime > 1){
		if(prime % 2 != 0 & prime % 3 != 0){
			break;
		}

		prime -= 1;
	}

	HashMap* newMap = malloc(sizeof(HashMap));

	if (newMap == nullptr) return newMap;

	newMap->prime       = prime;
	newMap->map_size    = map_size;
	newMap->textures    = malloc(sizeof(HashMap)* map_size);
	newMap->putMapValue = putMapValue;
	newMap->getMapValue = getMapValue;

	return newMap;
}

Texture2D getMapValue(HashMap *hashmap, const int key){
	//find the nearest prime number
	return hashmap->textures[key % hashmap->prime];
	//hashing function
}

void putMapValue(HashMap *hashmap, const int key, const Texture2D texture_value){
	hashmap->textures[key % hashmap->prime] = texture_value;
}
