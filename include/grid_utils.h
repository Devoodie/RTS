#include <stdio.h>

typedef struct hex_space {	
	//neighbors list
	struct hex_space *neighbors[6];
	int x;
	int y;
} HexSpace;

extern HexSpace default_hex; 
enum cardinals {
	NORTH_EAST = 0,
	EAST = 1,
	SOUTH_EAST = 2,
	SOUTH_WEST = 3,
	WEST = 4, 
	NORTH_WEST = 5,
};

void initialize_grid(int row, int col, HexSpace grid[row][col]);
