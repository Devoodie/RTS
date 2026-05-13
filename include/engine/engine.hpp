#ifndef RTS_ENGINE_H
#define RTS_ENGINE_H

#include "utils/grid.hpp"
#include <vector>

class Player {
	public:
		std::vector<Unit*> units;
	//structures Aidan ARRAYLIST
	//resources

		Player();
};

namespace engine {
	enum inputAlphabet {
		TURNEND,
		UNIT,
		HEX,
		MOVE,
	};

	enum states {
		IDLE,
		UNIT1,
		OPTIONS,
		HEX_INFO,
		UNIT_INFO,
		MOVING,
	};

	enum uiElem {
		UI_OPTIONS_1,
		UI_INFO,
	};

class Game{
	public:
		int player_index;
		int player_count;
		std::vector<Player> players;
		std::vector<Unit*> units;

		std::vector<std::vector<HexSpace>> grid_space;
		std::vector<Rectangle> ui_elements;

		Unit *selected_unit;
		Unit *selected_unit2;

		HexSpace *selected_hex;
		HexSpace *selected_hex2;
		states state;

		//REDUNDANT PROPERTY
		Vector2 MousePosition;

		void playerInit(int playerCount);
		void endTurn();

		//All Transition functions change state and state should only be changed in transition functions
		
		void hexInfoTransition(inputAlphabet input, void *selection);
		//transitions fsm from idle to another state
		void idleTransition(inputAlphabet input, void *selection);

		//transitions fsm from option to another state
		void optionTransition(inputAlphabet input, void *selection);

		void unitInfoTransition(inputAlphabet input, void *selection);
		//transitions fsm from unit to another state
		void unitTransition(inputAlphabet input, void *selection);

		//handles state transition calls 
		void transitionState(inputAlphabet input, void *selection);

		//transitions to idle state or calls menu
		void escape();

		//ui creation helper function
		void createUiElem(uiElem ui_type);

		//searches for collisions with properties of a hexagon and potentially transitions state
		void handleCollision(HexSpace *collided_hex, Vector2 mouse_point);

		//checks for collisions with hexagons then calls handle collision
		bool collisionCheck();

		bool uiCollisionCheck();

		void Move();

		//main game loop
		void versus();

		//renders options menu
		void renderOptions(std::unordered_map<int, Texture2D> texture_map);

		Game();
		
};


}

//class Collider {};
//
#endif
