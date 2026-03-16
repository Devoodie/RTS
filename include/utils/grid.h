#ifndef RTS_GRID_H
#define RTS_GRID_H

#include "../assets.h"

typedef struct hex_space {	
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


typedef struct hashmap {
	Texture2D *textures;

	int map_size;
	int prime;

	//size_t type_size;
	//int value_type;
	//
	Texture2D (*getMapValue)(const struct hashmap*, int key);
	void (*putMapValue)(struct hashmap*, int key, Texture2D);
} HashMap;

enum textures {
	GRASS_HEX = 0,
};

void initializeGrid(int row, int col, HexSpace grid[row][col]);

void renderGrid(HashMap textures, int row, int col, HexSpace grid[row][col], int debug);

void hashmapInitAssets(HashMap *texture_map);

//ALLOCATION
HashMap HashmapInit(int map_size);

Texture2D GetMapValue(const HashMap* hashmap, int key);

void putMapValue(HashMap *hashmap, int key, Texture2D texture_value);

#endif