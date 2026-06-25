#ifndef RTS_ENGINE_H
#define RTS_ENGINE_H

#include "utils/grid.hpp"
#include "entities.hpp"
#include <string>
#include <vector>
#include <cstdint>

class Player {
	public:
		std::vector<uint16_t> units;
		std::vector<uint8_t> buildings;

		//creates a player with an index to its HQ
		Player(int hq_index);
};

struct Text {
	std::string content;
	Color text_color;
	Vector2 position;
	float font_size;
};

//will provide information to autofill menus on creation
enum scroll_type {
	UNITS,
	UPGRADES,
};

class ScrollMenu {
	public:
		std::vector<Rectangle> elements;
		Rectangle dimensions;
		uint16_t y_pos;

		scroll_type type;
		ScrollMenu(scroll_type menu_type, Rectangle placement);
};

namespace engine {
	enum inputAlphabet {
		TURNEND,
		UNIT,
		HEX,
		BUILDING,
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
		SCROLL,
	};

	enum uiElem {
		UI_OPTIONS_1,
		UI_INFO,
		UI_FIRING_TEXT,
		INFANTRY_SCRL,
	};

class Game{
	public:	
		std::vector<Player> players;
		std::vector<Unit> units;
		std::vector<Building> buildings;

		std::vector<std::vector<HexSpace>> grid_space;
		std::vector<Rectangle> ui_elements;
		std::vector<Text> messages;
		ScrollMenu scrl_menu;

		int player_index;
		int player_count;

		//Temporary
		float dmg_taken;
		uint8_t dmg_txt_index;

		Unit *selected_unit;
		Unit *selected_unit2;

		HexSpace *selected_hex;
		HexSpace *selected_hex2;
		states state;

		Vector2 MousePosition;

		Camera2D &camera;

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
		//May create options or scroll menus
		//Mouse position must be set to adjust element position
		void createUiElem(uiElem ui_type);

		//searches for collisions with properties of a hexagon and potentially transitions state
		void handleCollision(HexSpace *collided_hex, Vector2 mouse_point);

		//checks for collisions with hexagons then calls handle collision
		bool collisionCheck();

		bool uiCollisionCheck();

		float calcDamage();

		//removes unit at index and decrement all indexes 
		void popUnit(uint16_t rm_index);

		void Move();
		
		void Fire();

		//main game loop
		void versus();

		Game(Camera2D &camera);
		
};
}

//
#endif
