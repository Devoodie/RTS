#ifndef RTS_GRID_H
#define RTS_GRID_H

#include <unordered_map>
#include <vector>

extern "C" {
#include"raylib.h"
}

class Unit;

class HexSpace {	
	public:
		HexSpace *neighbors[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
		float x;
		float y;
		Unit *occupier;

		Vector2 vertices[6];
	HexSpace();
};

namespace grid {

enum cardinals {
	NORTH_EAST = 0,
	EAST = 1,
	SOUTH_EAST = 2,
	SOUTH_WEST = 3,
	WEST = 4, 
	NORTH_WEST = 5,
};

extern float radius;
extern float inradius;



enum textures {
	GRASS_HEX = 0,
	GRASS_BORDER,
	DARK_SOLIDER,
	FIRE_BUTTON,
	MOVE_BUTTON,
};

void initGrid(const int row, const int col, std::vector<std::vector<HexSpace>> &grid);

void renderGrid(std::unordered_map<int, Texture2D> textures, std::vector<std::vector<HexSpace>> grid, int debug);

void initAssets(std::unordered_map<int , Texture2D> &texture_map);

}
#endif
