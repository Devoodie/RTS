#ifndef RTS_UNIT_H
#define RTS_UNIT_H

#include <stdio.h>
#include <stddef.h>

#include <grid.h>

enum unit_type {
	LAND,
	VEHICLE,
	AIR,
};

typedef struct unit {
	char* owner;
	int mv_speed;
	int atk_range;
	enum unit_type type;
	HexSpace* current_hex;
	bool can_fly;
} Unit;


extern Unit default_unit;

#endif
