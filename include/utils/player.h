#ifndef RTS_PLAYER_H
#define RTS_PLAYER_H

#include "arraylist.h"

typedef struct player  {
	ArrayList *units; 
	ArrayList *structures; 
	//resources
	//init 
} Player;

Player* initPlayer();

#endif
