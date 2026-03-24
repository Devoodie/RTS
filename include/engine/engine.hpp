#include "utils/grid.hpp"
#include <vector>

enum UnitType {
	INFANTRY,
	JET,
	TANK,
};

class Unit {
	public:
		int attack_range;
		int move_speed;
		HexSpace* current_hex;
		Vector2 position;

		Unit(HexSpace *hex, UnitType type);
};

class Player {
	public:
		std::vector<Unit> units;
	//structures Aidan ARRAYLIST
	//resources
};

namespace engine {
void versus(std::vector<Player> &player, int currentPlayer);
}

//class Collider {};
