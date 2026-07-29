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

enum Tasks{
	NONE,
	CAPTURING,
};

class Unit {
	public:
		Rectangle render_rect;
		HexSpace *current_hex;
		Vector2 position;
		UnitType type;
		Tasks task;

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
		Unit(HexSpace *hex, UnitType type, int player_index);

		//Copy assignment operator for vector
};

enum BuildingType{
	HQ,
	FACTORY,
	RESEARCH,
	AIRPORT,
};

class Building{
	public:
		HexSpace *hex;
		Rectangle render_rect;
		float hp;
		uint8_t owner_index;
		BuildingType type;

		Building(HexSpace *hex, BuildingType type, int owner_index);
};

#endif
