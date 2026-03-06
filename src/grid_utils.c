#include "../include/grid_utils.h"

#include <math.h>
#include <raylib.h>

#include "../include/assets.h"


HexSpace default_hex = {
	.x = 0,
	.y = 0,
	.neighbors = {NULL, NULL, NULL, NULL, NULL, NULL},
};

int ScreenWidth = 1920;  
int ScreenHeight = 1080;

float radius = 0;
float inradius = 0;

void initializeAssets(){
	grassHex = LoadTexture("../assets/Hex_Grass_Single.png");
}

void initializeGrid(int row, int col, HexSpace grid[row][col]){
	// i is our row (y) index
	// j is our column (x) index
	
	//hexagon math
	radius = ScreenWidth / 16;
	inradius = (radius * sqrtf(3.0)) / 2;

	// decide wether positional values should be normalized or not 
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
		y += radius * 2;
	}
}

void renderGrid(const int row, const int col, const HexSpace grid[row][col]){
	Rectangle drawing_rectangle = {
		.x = 0,
		.y = 0,
		.height = radius * 2,
		.width = inradius * 2,
	};

	for(int i = 0; i < row; ++i){
	    for(int j = 0; j < col; ++j){
		    const HexSpace *CurrentHex = &grid[i][j];
		    DrawCircle(CurrentHex->x, CurrentHex->y, 3.0, RED);
	//			    DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);
	    }
	}

}

