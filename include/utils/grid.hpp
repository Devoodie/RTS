#ifndef RTS_GRID_H
#define RTS_GRID_H

#include <unordered_map>
#include <vector>

extern "C" {
	#include "raylib.h"
}

class Unit;

namespace grid {
	class AStar;

	extern int ScreenWidth;
	extern int ScreenHeight;
	extern float radius;
	extern float inradius;

	enum cardinals {
		NORTH_EAST = 0,
		EAST = 1,
		SOUTH_EAST = 2,
		SOUTH_WEST = 3,
		WEST = 4,
		NORTH_WEST = 5,
	};

	enum textures {
		GRASS_HEX = 0,
		GRASS_BORDER,
		DARK_SOLIDER,
		FIRE_BUTTON,
		MOVE_BUTTON,
		INFO_RECT,
	};

	class HexSpace {
	public:
		HexSpace *neighbors[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
		float x_position;
		float y_position;

		int x_index{};
		int y_index{};

		Unit *occupier;
		int move_cost;

		Vector2 vertices[6]{};
		bool operator==(const HexSpace& other) const {
			return x_position == other.x_position && y_position == other.y_position;
		}

		HexSpace();
	};

	class Grid {
	public:
		AStar *astar;

		static void initGrid(
			int row,
			int col,
			std::vector<std::vector<HexSpace>> &grid);

		static void renderUnits(
			std::unordered_map<int, Texture2D> texture_map,
			std::vector<Unit*> units);

		static void renderGrid(
			std::unordered_map<int, Texture2D> textures,
			std::vector<std::vector<HexSpace>> grid,
			bool debug);

		static void initAssets(std::unordered_map<int , Texture2D> &texture_map);
	};
}
#endif
