#include <stdio.h>

#include <assets.h>


typedef struct hex_space {	
	//neighbors list
	struct hex_space *neighbors[6];
	float x;
	float y;
	//unit*
	//structures[]
} HexSpace;


enum cardinals {
	NORTH_EAST = 0,
	EAST = 1,
	SOUTH_EAST = 2,
	SOUTH_WEST = 3,
	WEST = 4, 
	NORTH_WEST = 5,
};

extern HexSpace default_hex; 
extern float radius;
extern float inradius;

void initializeGrid(const int row, const int col, HexSpace grid[row][col]);

void initializeAssets();

void renderGrid(const int row, const int col, const HexSpace grid[row][col]);
