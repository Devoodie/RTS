#include <engine/entities.hpp>

Unit::Unit(HexSpace *hex, UnitType type) {
	current_hex = hex;
	position = {
		.x = hex->x_position,
		.y = hex->y_position,
	};

	//add half of the width and height to the x and y (respectively)
	render_rect = {
		.x = hex->x_position - grid::inradius / 2,
		.y = hex ->y_position - grid::radius / 2,
		.width = grid::inradius,
		.height = grid::radius,
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
