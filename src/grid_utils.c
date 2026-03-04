# include "../include/grid_utils.h"

HexSpace default_hex = {
	.x = 0,
	.y = 0,
	.neighbors = {NULL, NULL, NULL, NULL, NULL, NULL},
};

void initialize_grid(int row, int col, HexSpace grid[row][col]){
	// i is our row (y) index
	// j is our column (x) index
	for(int i = 0; i < row; ++i){
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

		}
	}

}

