# include "../include/grid_utils.h"


void initialize_grid(int row, int col, HexSpace (*grid)[col]){
	for(int i = 0; i < row; ++i){
		for(int j = 0; j < col; j++){
			HexSpace *CurrentHex = &grid[i][j];
			*CurrentHex = default_hex;
			
			//cannot traverse off of the map 
			if(j == 0){
				CurrentHex->neighbors[3] = NULL;
				CurrentHex->neighbors[4] = NULL;
				CurrentHex->neighbors[5] = NULL;

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
	for(int i = 0; i < row; ++i){
		for(int j = 0; j < col; j++){
			HexSpace *CurrentHex = &grid[i][j];
			*CurrentHex = default_hex;
			
			//cannot traverse off of the map 
			if(j == 0){
				CurrentHex->neighbors[3] = NULL;
				CurrentHex->neighbors[4] = NULL;
				CurrentHex->neighbors[5] = NULL;

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
}

