#ifndef RTS_GRID_H
#define RTS_GRID_H

#include <unordered_map>
#include <vector>
#include <cstdint>

extern "C" {
#include"raylib.h"
}

class Unit;
class Building;
//MAJOR ISSUE ALL HEX'S WILL HAVE INVALID OCCUPIER POINTERS WHEN THE UNIT VECTOR REALLOCATES CHANGE TO INDEXING
class HexSpace {	
	public:
		HexSpace *neighbors[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
		Vector2 vertices[6];

		Vector2 indices;
		// 16 may not be necessary 
		uint16_t occupier_index;
		uint16_t structure_index;

		float x_position;
		float y_position;

		int move_cost;
		int env_defense;
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
	HQ,
	WAREHOUSE,
};

void initGrid(const int row, const int col, std::vector<std::vector<HexSpace>> &grid);

void renderBuildings(std::unordered_map<int, Texture2D> texture_map, const std::vector<Building> &buildings, const bool debug);

void renderUnits(std::unordered_map<int, Texture2D> texture_map, const std::vector<Unit> &units, const bool debug);

void renderGrid(std::unordered_map<int, Texture2D> textures, std::vector<std::vector<HexSpace>> grid, const bool debug);

void initAssets(std::unordered_map<int , Texture2D> &texture_map);

}
#endif
