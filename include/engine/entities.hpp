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
		int player_index; 
		int attack_range;
		int move_speed;
		HexSpace *current_hex;
		Vector2 position;
		Rectangle render_rect;

		Unit(HexSpace *hex, UnitType type);
};

#endif
