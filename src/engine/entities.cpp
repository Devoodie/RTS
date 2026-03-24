#include <engine/entities.hpp>
Unit::Unit(HexSpace *hex, UnitType type) {
	current_hex = hex;
	position = {
		.x = hex->x,
		.y = hex->y,
	};

	switch(type) {
		case INFANTRY:
			attack_range = 1;
			move_speed = 4;
		default:
			attack_range = 1;
			move_speed = 1;
	}
}
