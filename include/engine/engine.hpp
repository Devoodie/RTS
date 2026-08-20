#ifndef RTS_ENGINE_H
#define RTS_ENGINE_H

#include "utils/grid.hpp"
#include <utils/slotmap.hpp>
#include "entities.hpp"
#include "states.hpp"
#include "ui.hpp"
#include <string>
#include <vector>
#include <cstdint>
#include <memory>

class Player {
	public:
		std::vector<Slot>units;
		std::vector<Slot> buildings;

		//creates a player with an index to its HQ
		Player(Slot hq_key);
};

//will provide information to autofill menus on creation

namespace engine {
	enum inputAlphabet {
		TURNEND,
		UNIT,
		HEX,
		BUILDING,
		MOVE,
	};

class Game{
	public:	
		std::vector<Player> players;
		SlotMap<Unit> units;
		SlotMap<Building> buildings;

		std::vector<std::vector<HexSpace>> grid_space;
		ui::UiManager ui_manager;

		Slot state_element; //this is a state related element within ui_manager there should only be one at a time

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

		//Helper functions that put the engine into a state available from idle state.
		//ALL RETURN STATE TO SET
		
		//CREATES UI ELEMENTS AND RETURNS STATE TO SET 
		engine::states SelectBuilding(Building* building_ptr);

		//CREATES UI ELEMENTS AND PUTS THE STATE INTO EITHER UNIT1 OR UNIT INFO
		engine::states SelectUnit(Unit* unit_ptr);

		//All Transition functions change state and state should only be changed in transition functions
		//
		//transitions fsm from idle to another state
		void idleTransition(inputAlphabet input, void *selection);

		//transitions handles fire transitions option transitions
		void optionTransition(inputAlphabet input, void *selection);

		void hexInfoTransition(inputAlphabet input, void *selection);

		//transitions fsm from unit to another state
		void unitTransition(inputAlphabet input, void *selection);

		void unitInfoTransition(inputAlphabet input, void *selection);

		// void scrollTransition(inputAlphabet input, void *selection);

		//handles state transition calls 
		void transitionState(inputAlphabet input, void *selection);

		//transitions to idle state or calls menu
		void escape();

		//searches for collisions with properties of a hexagon and potentially transitions state
		void handleCollision(HexSpace *collided_hex, Vector2 mouse_point);

		//checks for collisions with hexagons then calls handle collision
		bool collisionCheck();

		//spawn unit depending on signal
		void spawnUnit(ui::UiSignal signal);

		void handleSignal(ui::UiSignal signal);

		float calcDamage();

		//removes unit at index and decrement all indexes 
		void eraseUnit(const Slot &key);

		void transferBuilding(Slot &building_index, int current_owner, int new_owner);

		void Move();
		
		void Fire();

		//main game loop
		void versus();

		Game(Camera2D &camera);	
};
}

//
#endif
