#include <iostream>
#include <raylib.h>
#include <engine/engine.hpp>
#include <engine/entities.hpp>


Player::Player(){
	units = std::vector<Unit>();
	units.reserve(12);
}

namespace engine {

	Game::Game(){
		players = std::vector<Player>();
		player_count = 0;
		player_index = 0;
		grid_space = std::vector<std::vector<HexSpace>>(8, std::vector<HexSpace>(8));
	}

	void Game::playerInit(int playerCount){
		if(players.size() == playerCount){
			std::cout << "PLAYER COUNT ALREADY MET!" << std::endl;
			return;
		}
		while(players.size() < playerCount) {
			players.emplace_back(Player());
		}
	}

	void Game::endTurn(){
		std::cout << "End Turn!" << std::endl;
		player_index = (player_index + 1) % player_count;
		//state transition

		return;
	}

	bool Game::uiCollisionCheck(){
		Vector2 mouse_point = GetMousePosition();
		for(int i = 0; i < ui_elements.size(); ++i){
			switch(i){
				//end turn
				case 0:
					if(CheckCollisionPointRec(mouse_point, ui_elements[0]) and IsMouseButtonDown(0)){
						return true;
					}
				default:
					//std::cerr << "UI element " << i << " Not Recognized" << std::endl;
					return false;
			};
		}
		return false;
	}

	void Game::idleTransition(inputAlphabet input, void *selection){
		switch(input){
			case TURNEND:
				break;
			case UNIT: {
				Unit *unit_ptr = (Unit*)selection;
				this->selected_unit = unit_ptr;
				this->selected_hex = unit_ptr->current_hex;
				break;
				   }
			case HEX:
				this->selected_hex = (HexSpace*)selection;
				break;
			default:
				std::cerr << "ERR: INPUT NOT FOUND (idleTransition)" << std::endl;
				return;

		}
	}

	void Game::transitionState(inputAlphabet input, void *selection){
		switch(this->state){
			case IDLE:
				idleTransition(input, selection);
				break;
			case UNIT1:
				break;
			case OPTIONS:
				break;
			default:
				std::cerr << "ERR: INPUT NOT FOUND (stateTransition)" << std::endl;
				return;
		}
	}

	void Game::handleCollision(HexSpace *collided_hex, Vector2 mouse_point){
		if(collided_hex->occupier != nullptr){
			if (CheckCollisionPointRec(mouse_point, collided_hex->occupier->collision_rec) and IsMouseButtonReleased(0)) {
				//do unit stuff
				std::cout << "Unit Collision Detected" << std::endl;
				this->transitionState(UNIT, collided_hex->occupier);
			}
			else if(IsMouseButtonReleased(0)) {
				this->transitionState(HEX, collided_hex);
			}
		} else {
			if (IsMouseButtonReleased(0)) this->transitionState(HEX, collided_hex);
		}
	}


	//aidan optimize search (I THINK BINARY SEARCH WILL SHINE HERE)
	bool Game::collisionCheck(){
		Vector2 mouse_point = GetMousePosition();
		for(int i = 0; i < grid_space.size(); ++i){
			for(int j = 0; j < grid_space[i].size(); ++j){
				HexSpace *CurrentHex = &grid_space[i][j];

				//Hexagon Hover (NO HOVER)
				if(CheckCollisionPointPoly(mouse_point, CurrentHex->vertices, 6) and IsMouseButtonDown(0)) {
					std::cout << "Collision Hex " << j << ", " << i <<std::endl;

					this->handleCollision(CurrentHex, mouse_point);
					return true;
				}
			}
		}
		return false;
	}

	void Game::versus(){
		if(players.size() == 0) playerInit(player_count);

		Player current_player = players[player_index];

		//check collisions
		//uichecks

		bool ui_state = this->uiCollisionCheck();
		bool chng_state = this->collisionCheck();


		//check_hexagon
	}
}
