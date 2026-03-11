#include <stdio.h>
#include <stddef.h>

#include "./assets.h"

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
	Texture2D (*getMapValue)(struct hashmap*, int key);
	void (*putMapValue)(struct hashmap*, int key, Texture2D);
} HashMap;

enum textures {
	GRASS_HEX = 0,
};

void initializeGrid(const int row, const int col, HexSpace grid[row][col]);

void renderGrid(HashMap textures, const int row, const int col, const HexSpace grid[row][col], int debug);

void hashmap_initAssets(HashMap *texture_map);

//ALLOCATION
HashMap* hashmap_init(int map_size);

Texture2D getMapValue(HashMap* hashmap, int key);

void putMapValue(HashMap *hashmap, int key, Texture2D texture_value);
