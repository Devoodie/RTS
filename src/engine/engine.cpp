#include <cstdlib>
#include <iostream>
#include <cassert>
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
				//temporary
			case UI_FIRING_TEXT:
				this->ui_elements.emplace_back((Rectangle){
					.x = float(selected_unit2->position.x - (selected_unit2->render_rect.width / 4)),
					.y = this->selected_unit2->position.y,
					.width = 255,
					.height = 0,
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

				bool unit_owned = unit_ptr->player_index == this->player_index;

				//HEX INFO NEEDS THIS TOO
				if(unit_owned){
					this->selected_unit = unit_ptr;
					this->selected_hex = unit_ptr->current_hex;
					this->state = UNIT1;

				} else {
					//SHOW COMPARISON
					//WORK HERE
					this-> selected_unit = unit_ptr;
					this->state = UNIT_INFO;
					this->createUiElem(UI_INFO);
				}
				break;
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
				break;
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
				bool unit_owned = unit_ptr->player_index == this->player_index;

				if(unit_owned){
				} else if(std::abs(hex_ptr->indices.x - selected_hex->indices.x) <= selected_unit->attack_range and
					  std::abs(hex_ptr->indices.y - selected_hex->indices.y) <= selected_unit->attack_range) {

					this->selected_unit2 = unit_ptr;

					Vector2 button_position = unit_ptr->position;
					this->MousePosition = button_position;
					this->createUiElem(UI_OPTIONS_1);
					
					//PROB DO FIRE ANIMATION
					this->state = FIRE;

					//i don't want to put this before every single append
					//this->ui_elements.erase(ui_elements.begin() + 1);
					std::cout << "FIREEEEEEE" <<std::endl;
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

	//can't decide if this belongs in engine or unit
	float Game::calcDamage(){
		//base damage when attack = defense is 40
		int diff = this->selected_unit2->defense - this->selected_unit->cmbt_str;
		float dmg = 40.0;
		//reduction/enhancement based upon defense
		float multipier = 1.0;

		if(diff > 3){
			float x = ((diff - 3) * .6) + .6;
			multipier = (4 * x) / (5 * x + 1);
		} else if(diff > 0){
			multipier = diff * .2;
		} else if(diff < 0){
			//increase damage
		}

		//apply environmental defense
		dmg *= multipier;
		return dmg;
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
		if(CheckCollisionPointRec(mouse_point, ui_elements[1]) and IsMouseButtonReleased(0)){
			switch(this->state){
				//end turn
				case OPTIONS:
						selected_unit->position.x = selected_hex2->x_position;
						selected_unit->position.y = selected_hex2->y_position;
						selected_unit->current_hex = selected_hex2;

						this->selected_hex->occupier = nullptr;
						this->selected_hex2->occupier = selected_unit;

						this->state = MOVING;
						std::cout << "MOVE!" << std::endl;
						break;
					
				case FIRE:
						assert("Number of atks Greater than 0" && this->selected_unit->atks_left > 0);
						this->selected_unit->atks_left -= 1;

						break;
				default:
					//std::cerr << "UI element " << i << " Not Recognized" << std::endl;
					return false;
			}
			return true;
		}
		return false;
	}

	//moves units
	void Game::Move(){
		Unit *unit_ptr = this->selected_unit;

		Vector2 destRect = {
			.x = unit_ptr->position.x - grid::inradius / 2,
			.y = unit_ptr->position.y - grid::radius / 2,
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
//WORK HERE
	void Game::Fire(){
		Color fire_text = ColorAlpha(RED, fire_txt_alpha);

		Vector2 text_position = {
			.x = ui_elements[1].x,
			.y = ui_elements[1].y,
		};

		// Vector2 destination = {
		// 	.x = selected_unit2->position.x,
		// 	.y = ,
		// };

		DrawTextEx(
			GetFontDefault(), 
			TextFormat("%s Took %i Damage from %s!", selected_unit2->name.c_str(), this->dmg_taken, selected_unit->name.c_str()),
			text_position, 
			15, 
			0, 
			fire_text
			);

		//NEEDS CHANGES
		float max_dist = 25 * GetFrameTime();
//		Vector2 new_coords = Vector2MoveTowards(text_position, Vector2 target, float maxDistance);
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

}
