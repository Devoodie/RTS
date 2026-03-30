#include <engine/entities.hpp>

Unit::Unit(HexSpace *hex, UnitType type) {
	current_hex = hex;
	position = {
		.x = hex->x,
		.y = hex->y,
	};

	//add half of the width and height to the x and y (respectively)
	collision_rec = {
		.x = hex->x,
		.y = hex ->y,
		.width = grid::inradius / 2,
		.height = grid::radius / 2,
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
