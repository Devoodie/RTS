#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <engine/engine.hpp>
#include <engine/entities.hpp>
#include <unordered_map>

Player::Player(){
	units = std::vector<Unit>();
	units.reserve(12);
}

namespace engine {

	Game::Game(){
		players = std::vector<Player>();
		player_count = 2;
		player_index = 0;
		grid_space = std::vector<std::vector<HexSpace>>(8, std::vector<HexSpace>(8));
		state = IDLE;
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

	//ADD STATE CHANGES
	void Game::endTurn(){
		std::cout << "End Turn!" << std::endl;
		player_index = (player_index + 1) % player_count;
		//state transition

		return;
	}

	void Game::escape(){
		this->state = IDLE;
		this->selected_unit = nullptr;
		this->selected_unit2 = nullptr;

		this->selected_hex = nullptr;
		this->selected_hex2 = nullptr;

		if(this->ui_elements.size() > 1) this->ui_elements.erase(ui_elements.begin() + 1);
	}

	void Game::idleTransition(inputAlphabet input, void *selection){
		switch(input){
			case TURNEND:
				break;
			case UNIT: {
				std::cout << "Unit Selected" << std::endl;
				Unit *unit_ptr = (Unit*)selection;
				this->selected_unit = unit_ptr;
				this->selected_hex = unit_ptr->current_hex;
				this->state = UNIT1;
				break;
				   }
			case HEX:
				this->selected_hex = (HexSpace*)selection;
//				this->MousePosition = GetMousePosition();
//				this->state = INFO;
				break;
			default:
				std::cerr << "ERR: INPUT NOT FOUND (idleTransition)" << std::endl;
				return;

		}
	}

	void Game::optionTransition(inputAlphabet input, void *selection){
		switch(input){
			case UNIT:
				break;
			case HEX:
				//WRONG WORK HERE
				this->selected_hex = (HexSpace*)selection;
				this->MousePosition = GetMousePosition();

				this->ui_elements[1].x = MousePosition.x + grid::inradius / 4;
				this->ui_elements[1].y = MousePosition.y;
				break;
			default:
				return;
		}
	}

	
	void Game::unitTransition(inputAlphabet input, void *selection){
		switch(input){
			case UNIT:{
				Unit *new_unit = (Unit*)selection;
				if(new_unit == selected_unit) this->escape();
				break;
				  }
			case HEX:
				std::cout << "Hex 2 Selected" << std::endl;
				this->MousePosition = GetMousePosition();
				this->selected_hex2 = (HexSpace*) selection;
				this->state = OPTIONS;

				//create ui element for options
				this->ui_elements.emplace_back((Rectangle){
					.x = this->MousePosition.x + grid::inradius / 4,
					.y = this->MousePosition.y,
					.width = grid::inradius,
					.height = grid::radius / 2,
					});
				break;
			default:
				return;
		}
	}

	void Game::transitionState(inputAlphabet input, void *selection){
		switch(this->state){
			case IDLE:
				idleTransition(input, selection);
				break;
			case UNIT1:
				unitTransition(input, selection);
				break;
			case OPTIONS:
				optionTransition(input, selection);
				break;
			default:
				std::cerr << "ERR: INPUT NOT FOUND (stateTransition)" << std::endl;
				return;
		}
	}

	void Game::handleCollision(HexSpace *collided_hex, Vector2 mouse_point){
		if(collided_hex->occupier != nullptr){
			//CHANGE ALL TO RELEASED OR DOWN (THEY MUST ALL BE THE SAME)
			if (CheckCollisionPointRec(mouse_point, collided_hex->occupier->render_rect) and IsMouseButtonReleased(0)) {
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
				if(CheckCollisionPointPoly(mouse_point, CurrentHex->vertices, 6) and IsMouseButtonReleased(0)) {
					std::cout << "Collision Hex " << j << ", " << i <<std::endl;

					this->handleCollision(CurrentHex, mouse_point);
					return true;
				}
			}
		}
		return false;
	}

	bool Game::uiCollisionCheck(){
		Vector2 mouse_point = GetMousePosition();
		//0 endturn button, 1 Others(????), 
		//may need to change the way this works to switch on states instead of ui elements
		for(int i = 0; i < ui_elements.size(); ++i){
			switch(i){
				//end turn
				case 0:
					if(CheckCollisionPointRec(mouse_point, ui_elements[0]) and IsMouseButtonReleased(0)){
						std::cout << "ENDTURN" << std::endl;
						this->endTurn();
						return true;
					}
				case 1:
					if(this->ui_elements.size() < 2) continue;
					if(CheckCollisionPointRec(mouse_point, ui_elements[1]) and IsMouseButtonReleased(0)){
						selected_unit->position.x = selected_hex2->x;
						selected_unit->position.y = selected_hex2->y;
						this->state = MOVING;
						std::cout << "MOVE!" << std::endl;
						return true;
					}
				default:
					//std::cerr << "UI element " << i << " Not Recognized" << std::endl;
					return false;
			};
		}
		return false;
	}

	//moves units
	void Game::Move(){
		Unit *unit = this->selected_unit;

		Vector2 destRect = {
			.x = unit->position.x - grid::inradius / 2,
			.y = unit->position.y - grid::radius  / 2,
		};

		if(destRect.x != unit->render_rect.x or destRect.y != unit->render_rect.y){

//			std::cout << "Position: (" << unit->render_rect.x << "," << unit->render_rect.y << ")\n" <<"Desired Position: (" << destRect.x << " ," << destRect.y << ")" << std::endl;
			Vector2 renderRect = {
				.x = unit->render_rect.x, 
				.y = unit->render_rect.y
			};

			float max_dist = GetFrameTime() * 240;
			Vector2 new_pos = Vector2MoveTowards(renderRect, destRect, max_dist);

			unit->render_rect.x = new_pos.x;
			unit->render_rect.y = new_pos.y;
		} else {
			selected_hex->occupier = NULL;
			selected_hex2->occupier = unit;
			escape();
		}
	}

	void Game::versus(){
		if(players.size() == 0) playerInit(player_count);

		Player current_player = players[player_index];

		//check collisions
		//uichecks

		if(IsKeyPressed(KEY_ESCAPE)) {
			this->escape();
			return;
		}

		if(this->state == MOVING){
			this->Move();
			return;
			//dothis
		}
		bool ui_collision = this->uiCollisionCheck();
		
		if(!ui_collision) {
			bool chng_state = this->collisionCheck();
		}

		//check_hexagon
	}

	//THIS SHOULD BE CHANGED FROM RENDER OPTIONS TO RENDER UI
	//CHECK NULL POINTERS TO DETERMINE WHICH RECTANGLES SHOULD BE RENDERED
	void Game::renderOptions(std::unordered_map<int, Texture2D> texture_map){
		if(this->state != OPTIONS){
			return;
		} else {
			Texture2D &move_button = texture_map[grid::MOVE_BUTTON];

			Rectangle texture_rect = {
				.x = 0,
				.y = 0,
				.width = (float)move_button.width,
				.height = (float)move_button.height,
			};

			Rectangle options = this->ui_elements[1];
			DrawTexturePro(move_button, texture_rect, options, (Vector2){.x = 0, .y = 0}, 0, RAYWHITE);
		}
	}
}
