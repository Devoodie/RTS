#ifndef RTS_GRID_H
#define RTS_GRID_H

#include <unordered_map>
#include <vector>

extern "C" {
#include "raylib.h"
}

class Unit;

class HexSpace {	
	public:
		HexSpace *neighbors[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
		float x;
		float y;
		Unit *occupier;
		int move_cost;

		Vector2 vertices[6];

		bool operator==(const HexSpace& other) const {
			return x == other.x && y == other.y;
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

extern float radius;
extern float inradius;



enum textures {
	GRASS_HEX = 0,
	GRASS_BORDER,
	DARK_SOLIDER,
	FIRE_BUTTON,
	MOVE_BUTTON,
};

/// Calculates the shortest past from source to destination
/// @param source The source HexSpace
/// @param destination The destination HexSpace
/// @param grid The grid containing the source and destination HexSpace
/// @return The path from source to destination, will only contain source on error
std::vector<HexSpace*> astar(
	HexSpace &source,
	const HexSpace &destination,
	std::vector<std::vector<HexSpace>> &grid);

void initGrid(const int row, const int col, std::vector<std::vector<HexSpace>> &grid);

void renderUnits(std::unordered_map<int, Texture2D> texture_map, std::vector<Unit*> units);

void renderGrid(std::unordered_map<int, Texture2D> textures, std::vector<std::vector<HexSpace>> grid,const bool debug);

void initAssets(std::unordered_map<int , Texture2D> &texture_map);

}
#endif
