#ifndef RTS_ENGINE_H
#define RTS_ENGINE_H

#include "utils/grid.hpp"
#include <vector>

class Player {
	public:
		std::vector<Unit> units;
	//structures Aidan ARRAYLIST
	//resources

		Player();
};

namespace engine {
	enum inputAlphabet {
		TURNEND,
		UNIT,
		HEX,
	};

	enum states {
		IDLE,
		UNIT1,
		OPTIONS,
	};

class Game{
	public:
		int player_index;
		int player_count;
		std::vector<Player> players;
		std::vector<std::vector<HexSpace>> grid_space;
		std::vector<Rectangle> ui_elements;

		states state;

		void stateTransition(inputAlphabet input);

		void playerInit(int playerCount);
		void endTurn();
		bool uiCollisionCheck();
		bool collisionCheck();
		void versus();

		Game();
};

void handleCollision(HexSpace collided_hex);


}

//class Collider {};
//
#endif
