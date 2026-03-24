#ifndef RTS_ENTITIES_H
#define RTS_ENTITIES_H 

#include <utils/grid.hpp>

class HexSpace;

enum UnitType {
	INFANTRY,
	JET,
	TANK,
};

class Unit {
	public:
		int attack_range;
		int move_speed;
		HexSpace *current_hex;
		Vector2 position;

		Unit(HexSpace *hex, UnitType type);
};

#endif
