#include <stdio.h>

typedef struct hex_space {	
	//neighbors list
	struct hex_space *neighbors[6];
	int x;
	int y;
} HexSpace;

HexSpace default_hex = {
	.x = 0,
	.y = 0,
	.neighbors = {NULL, NULL, NULL, NULL, NULL, NULL},
};

enum cardinals {
	NORTH_EAST = 0,
	EAST = 1,
	SOUTH_EAST = 2,
	SOUTH_WEST = 3,
	WEST = 4, 
	NORTH_WEST = 5,
};

void initialize_grid(int row, int col, HexSpace grid[row][col]){
	for(int i = 0; i < row; ++i){
		for(int j = 0; j < col; j++){
			HexSpace *CurrentHex = &grid[i][j];
			*CurrentHex = default_hex;
			
			//cannot traverse off of the map 
			if(j == 0){
				*CurrentHex.neighbors[3] = NULL;
				*CurrentHex.neighbors[4] = NULL;
				*CurrentHex.neighbors[5] = NULL;
			} else {
				void;
			}

			if(j == col - 1){

			} else {

			}

			if(i == 0){

			} else {

			}

			if(i == row - 1){

			} else {

			}

		}
	}

	void;
};
