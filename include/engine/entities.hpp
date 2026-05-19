#ifndef RTS_ENTITIES_H
#define RTS_ENTITIES_H 

#include <utils/grid.hpp>
#include <string>

class HexSpace;

enum UnitType {
	INFANTRY,
	JET,
	TANK,
};

class Unit {
	public:
		std::string name;
		int player_index; 

		//stats 
		float hp;
		int attacks;
		int attack_range;
		int move_speed;
		int cmbt_str;
		int defense;

		int atks_left;
		int mvs_left;

		HexSpace *current_hex;
		Vector2 position;
		Rectangle render_rect;

		Unit(HexSpace *hex, UnitType type, int index);
};

#endif
