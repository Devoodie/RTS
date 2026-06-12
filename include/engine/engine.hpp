#ifndef RTS_ENGINE_H
#define RTS_ENGINE_H

#include "utils/grid.hpp"
#include <string>
#include <vector>
#include <cstdint>

class Player {
	public:
		std::vector<uint16_t> units;
	//resources

		Player();
};

struct Text {
	std::string content;
	Color text_color;
	Vector2 position;
	float font_size;
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
		FIRE,
		FIRING,
		HEX_INFO,
		UNIT_INFO,
		MOVING,
	};

	enum uiElem {
		UI_OPTIONS_1,
		UI_INFO,
		UI_FIRING_TEXT,
	};

class Game{
	public:
		int player_index;
		int player_count;
		std::vector<Player> players;
		std::vector<Unit> units;

		std::vector<std::vector<HexSpace>> grid_space;
		std::vector<Rectangle> ui_elements;
		std::vector<Text> messages;

		//TEMPORARY
		float fire_txt_alpha;
		float dmg_taken;
		uint8_t dmg_txt_index;

		Unit *selected_unit;
		Unit *selected_unit2;

		HexSpace *selected_hex;
		HexSpace *selected_hex2;
		states state;

		Vector2 MousePosition;

		void playerInit(int playerCount);
		void endTurn();

		//All Transition functions change state and state should only be changed in transition functions
		
		//transitions fsm from idle to another state
		void idleTransition(inputAlphabet input, void *selection);

		//transitions handles fire transitions option transitions
		void optionTransition(inputAlphabet input, void *selection);

		void hexInfoTransition(inputAlphabet input, void *selection);

		//transitions fsm from unit to another state
		void unitTransition(inputAlphabet input, void *selection);

		void unitInfoTransition(inputAlphabet input, void *selection);

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

		float calcDamage();

		bool uiCollisionCheck();

		void Move();
		
		void Fire();

		//main game loop
		void versus();

		Game();
		
};
}

//
#endif
