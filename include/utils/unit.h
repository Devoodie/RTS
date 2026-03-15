#include <stdio.h>
#include <stddef.h>

#include "grid.h"

typedef struct unit {
	int atk_range;
	int move_range;
	HexSpace* current_hex;
	char* owner;
	bool can_fly;
} Unit;



extern Unit default_unit;
