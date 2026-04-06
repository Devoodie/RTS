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
		Unit *selected_unit;
		Unit *selected_unit2;

		HexSpace *selected_hex;
		HexSpace *selected_hex2;
		states state;

		Vector2 MousePosition;

		void playerInit(int playerCount);
		void endTurn();

		void idleTransition(inputAlphabet input, void *selection);
		void transitionState(inputAlphabet input, void *selection);

		bool uiCollisionCheck();

		void handleCollision(HexSpace *collided_hex, Vector2 mouse_point);
		bool collisionCheck();
		void versus();

		void renderOptions();

		Game();
		
};


}

//class Collider {};
//
#endif
