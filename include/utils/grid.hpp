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
		float x_position;
		float y_position;

		int x_index;
		int y_index;

		Unit *occupier;
		int move_cost;

		Vector2 vertices[6];
		bool operator==(const HexSpace& other) const {
			return x_position == other.x_position && y_position == other.y_position;
		}

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

extern int ScreenWidth;
extern int ScreenHeight;
extern float radius;
extern float inradius;

enum textures {
	GRASS_HEX = 0,
	GRASS_BORDER,
	DARK_SOLIDER,
	FIRE_BUTTON,
	MOVE_BUTTON,
	INFO_RECT,
};

void initGrid(const int row, const int col, std::vector<std::vector<HexSpace>> &grid);

void renderUnits(std::unordered_map<int, Texture2D> texture_map, std::vector<Unit*> units);

void renderGrid(std::unordered_map<int, Texture2D> textures, std::vector<std::vector<HexSpace>> grid,const bool debug);

void initAssets(std::unordered_map<int , Texture2D> &texture_map);

}
#endif
