#ifndef RTS_ENTITIES_H
#define RTS_ENTITIES_H 

#include <utils/grid.hpp>
#include <string>
#include <cstdint>

class HexSpace;

enum UnitType {
	INFANTRY,
	JET,
	TANK,
};

class Unit {
	public:
		Rectangle render_rect;
		HexSpace *current_hex;
		Vector2 position;
		UnitType type;

		//stats 
		float hp;
		uint8_t attacks;
		uint8_t attack_range;
		uint8_t move_speed;
		uint8_t cmbt_str;
		uint8_t defense;

		uint8_t atks_left;
		uint8_t mvs_left;

		uint8_t owner_index; 

		//creates unit on hex
		Unit(HexSpace *hex, UnitType type, int index);

		//Copy assignment operator for vector
};

enum StructureType {
	HQ,
	FACTORY,
	RESEARCH,
	AIRPORT,
};

class Structure{
	public:
		StructureType type;
};

#endif
