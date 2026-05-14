#include <cstdlib>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <engine/engine.hpp>
#include <engine/entities.hpp>
#include <unordered_map>
#include <algorithm>

Player::Player(){
	units = std::vector<Unit*>();
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
		this->escape();
		player_index = (player_index + 1) % player_count;
		std::cout << "Player Index: " << player_index;
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

	void Game::createUiElem(uiElem ui_type){
		switch(ui_type){
			case UI_OPTIONS_1:
				this->ui_elements.emplace_back((Rectangle){
					.x = this->MousePosition.x + grid::inradius / 4,
					.y = this->MousePosition.y,
					.width = grid::inradius,
					.height = grid::radius / 2,
					});
				break;
			case UI_INFO:
				this->ui_elements.emplace_back((Rectangle){
					.x = float((grid::ScreenWidth * 3) / 4 ),
					.y = 0,
					.width = float(grid::ScreenWidth / 4),
					.height = float(grid::ScreenHeight),
					});
				break;
		}
	}

	void Game::hexInfoTransition(inputAlphabet input, void *selection){
		switch(input){
			case HEX:{
				HexSpace* hex_ptr = (HexSpace*)selection;
				if(this->selected_hex == hex_ptr) {
					this->escape();
					return;
				}
				this->selected_hex = (HexSpace*)selection;
				break;
				 }
			case UNIT:
				std::cout << "Unit Selected" << std::endl;
				Unit *unit_ptr = (Unit*)selection;
				this->selected_unit = unit_ptr;
				this->selected_hex = unit_ptr->current_hex;
				this->state = UNIT1;
				if(this->ui_elements.size() > 1) this->ui_elements.erase(ui_elements.begin() + 1);
				break;
			defaut:
				std::cerr << "STATE " << input << " NOT HANDLED (hexInfoTransition)" << std::endl;
		}
	}

	void Game::idleTransition(inputAlphabet input, void *selection){
		switch(input){
			case TURNEND:
				break;
			case UNIT: {
				std::cout << "Unit Selected" << std::endl;
				Unit *unit_ptr = (Unit*)selection;

				bool unit_owned = false;
				//HEX INFO NEEDS THIS TOO
				for(int i = 0; i < this->players[player_index].units.size(); ++i){
					Unit *current_ptr = players[player_index].units[i];
					if(current_ptr == unit_ptr){
						unit_owned = true;
						break;
					}
				}

				if(unit_owned){
					this->selected_unit = unit_ptr;
					this->selected_hex = unit_ptr->current_hex;
					this->state = UNIT1;
					break;
				} else {
					//SHOW COMPARISON
					//WORK HERE
					this->state = UNIT_INFO;
				}
				   }
			case HEX:
				this->selected_hex = (HexSpace*)selection;
				this->state = HEX_INFO;
				this->createUiElem(UI_INFO);
				break;
			default:
				std::cerr << "ERR: INPUT NOT FOUND (idleTransition)" << std::endl;
				return;

		}
	}

	void Game::optionTransition(inputAlphabet input, void *selection){
		switch(input){
			case UNIT:{
				Unit* unit_ptr = (Unit*)selection;
				if(this->selected_unit == unit_ptr){
					this->escape();
					return;
				} 
				break;
				  }
			case HEX:{
				//WRONG WORK HERE
				HexSpace *hex_ptr = (HexSpace*)selection;
				if(hex_ptr->occupier != nullptr){ 
					this->ui_elements[1].x = 16384;
					this->ui_elements[1].y = 16384;
					return;
				};
				this->selected_hex2 = hex_ptr;
				this->MousePosition = GetMousePosition();

				this->ui_elements[1].x = MousePosition.x + grid::inradius / 4;
				this->ui_elements[1].y = MousePosition.y;
				break;
				 }
			default:
				return;
		}
	}

	void Game::unitInfoTransition(inputAlphabet input, void *selection){
		switch(input){
			case UNIT:{
				Unit *unit_ptr = (Unit*)selection;
				if(unit_ptr == this->selected_unit){
					this->escape();
					return;
				}

				//UNIT COMPARISON
				//this->selected_unit2 = unit_ptr;
				  }
			case HEX:
				std::cout << "Hex 2 Selected" << std::endl;
				HexSpace* hex_ptr = (HexSpace*) selection;
				if(hex_ptr->occupier != nullptr) return; 

				this->selected_hex2 = hex_ptr;
				this->MousePosition = GetMousePosition();
				this->state = OPTIONS;

				if(this->ui_elements.size() > 1) this->ui_elements.erase(ui_elements.begin() + 1);
				this->createUiElem(UI_OPTIONS_1);
				break;

		}
	}
	void Game::unitTransition(inputAlphabet input, void *selection){
		switch(input){
			case UNIT:{
				Unit *unit_ptr = (Unit*)selection;
				if(unit_ptr == selected_unit) this->escape();

				HexSpace* hex_ptr = unit_ptr->current_hex;
				const std::vector<Unit*> &players_units = this->players[this->player_index].units;
				bool unit_owned = std::ranges::contains(players_units.begin(), players_units.end(), unit_ptr);

				if(unit_owned){
				} else if(std::abs(hex_ptr->indices.x - selected_hex->indices.x) <= selected_unit->attack_range and
					  std::abs(hex_ptr->indices.y - selected_hex->indices.y) <= selected_unit->attack_range) {

					//WORKHERE
					std::cout << "FIREEEEEEE" <<std::endl;
					Vector2 button_position = unit_ptr->position;
					this->createUiElem(UI_OPTIONS_1);
					this->state = FIRE;
				}
				break;
				  }
			case HEX:{
				std::cout << "Hex 2 Selected" << std::endl;
				HexSpace *hex_ptr = (HexSpace*) selection;
				
				if(hex_ptr->occupier != nullptr) return;

				this->MousePosition = GetMousePosition();
				this->selected_hex2 = hex_ptr;
				this->state = OPTIONS;

				//create ui element for options
				this->createUiElem(UI_OPTIONS_1);
				break;
				 }
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
			case HEX_INFO:
				hexInfoTransition(input, selection);
				break;
			case UNIT_INFO:
				unitInfoTransition(input, selection);
				break;
			default:
				std::cerr << "ERR STATE " << input << " NOT FOUND (transitionState)" << std::endl;
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
		
		if(CheckCollisionPointRec(mouse_point, ui_elements[0]) and IsMouseButtonReleased(0)){
			std::cout << "ENDTURN" << std::endl;
			this->endTurn();
			return true;
		}
		if(this->ui_elements.size() < 2) return false;

		switch(this->state){
			//end turn
			case OPTIONS:
				if(CheckCollisionPointRec(mouse_point, ui_elements[1]) and IsMouseButtonReleased(0)){
					selected_unit->position.x = selected_hex2->x_position;
					selected_unit->position.y = selected_hex2->y_position;
					selected_unit->current_hex = selected_hex2;

					this->selected_hex->occupier = nullptr;
					this->selected_hex2->occupier = selected_unit;

					this->state = MOVING;
					std::cout << "MOVE!" << std::endl;
					return true;
				}
			default:
				//std::cerr << "UI element " << i << " Not Recognized" << std::endl;
				return false;
		};
	return false;
	}

	//moves units
	void Game::Move(){
		Unit *unit_ptr = this->selected_unit;

		Vector2 destRect = {
			.x = unit_ptr->position.x - grid::inradius / 2,
			.y = unit_ptr->position.y - grid::radius  / 2,
		};

		if(destRect.x != unit_ptr->render_rect.x or destRect.y != unit_ptr->render_rect.y){

//			std::cout << "Position: (" << unit_ptr->render_rect.x << "," << unit_ptr->render_rect.y << ")\n" <<"Desired Position: (" << destRect.x << " ," << destRect.y << ")" << std::endl;
			Vector2 renderRect = {
				.x = unit_ptr->render_rect.x, 
				.y = unit_ptr->render_rect.y
			};

			float max_dist = GetFrameTime() * 240;
			Vector2 new_pos = Vector2MoveTowards(renderRect, destRect, max_dist);

			unit_ptr->render_rect.x = new_pos.x;
			unit_ptr->render_rect.y = new_pos.y;
		} else {
			selected_hex->occupier = NULL;
			selected_hex2->occupier = unit_ptr;
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
		switch(this->state){
			case OPTIONS:{
				Texture2D &move_button = texture_map[grid::MOVE_BUTTON];

				Rectangle texture_rect = {
				     .x = 0,
				     .y = 0,
				     .width = (float)move_button.width,
				     .height = (float)move_button.height,
				};

			        Rectangle options = this->ui_elements[1];
			        DrawTexturePro(move_button, texture_rect, options, (Vector2){.x = 0, .y = 0}, 0, RAYWHITE);
				break;
				}
			case FIRE:{
				Texture2D &move_button = texture_map[grid::FIRE_BUTTON];

				Rectangle texture_rect = {
				     .x = 0,
				     .y = 0,
				     .width = (float)move_button.width,
				     .height = (float)move_button.height,
				};

			        Rectangle options = this->ui_elements[1];
			        DrawTexturePro(move_button, texture_rect, options, (Vector2){.x = 0, .y = 0}, 0, RAYWHITE);
				break;
				  }
			case UNIT_INFO:
			case HEX_INFO:{
				Texture2D &info_rect= texture_map[grid::INFO_RECT];

				Rectangle texture_rect = {
				     .x = 0,
				     .y = 0,
				     .width = (float)info_rect.width,
				     .height = (float)info_rect.height,
				};

			        Rectangle options = this->ui_elements[1];
			        DrawTexturePro(info_rect, texture_rect, options, (Vector2){.x = 0, .y = 0}, 0, RAYWHITE);
				}
				break;
			default:
				break;
		}
	}
}
