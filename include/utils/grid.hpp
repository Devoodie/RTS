#ifndef RTS_GRID_H
#define RTS_GRID_H

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <utils/slotmap.hpp>
#include <cmath>

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
		std::optional<Slot> occupier_key;
		std::optional<Slot> structure_key;

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

constexpr int ScreenWidth = 1920;  
constexpr int ScreenHeight = 1080;

static float radius = ScreenWidth / 24.0;
static float inradius = (radius * std::sqrtf(3.0) / 2.0);

enum textures {
	kGrassHex = 0,
	kGrassBorder,
	kDarkSolider,
	kFireButton,
	kFireButtonUnusable,
	kMoveButton,
	kMoveButtonUnusable,
	kCaptureButton,
	kCaptureButtonUnusable,
	kEndButton,
	kInfoRect,
	kHQ,
	kWarehouse,
};

void initGrid(const int row, const int col, std::vector<std::vector<HexSpace>> &grid);

void renderBuildings(std::unordered_map<int, Texture2D> texture_map, const SlotMap<Building> &buildings, const bool debug);

void renderUnits(std::unordered_map<int, Texture2D> texture_map, const SlotMap<Unit> &units, const bool debug);

void renderGrid(std::unordered_map<int, Texture2D> textures, std::vector<std::vector<HexSpace>> grid, const bool debug);

void initAssets(std::unordered_map<int , Texture2D> &texture_map);

}
#endif
