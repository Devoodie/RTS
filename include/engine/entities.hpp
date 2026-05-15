#ifndef RTS_ENTITIES_H
#define RTS_ENTITIES_H 

#include <utils/grid.hpp>

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
		grid::HexSpace *current_hex;
		Vector2 position;
		Rectangle render_rect;

		Unit(grid::HexSpace *hex, UnitType type);
};

#endif
