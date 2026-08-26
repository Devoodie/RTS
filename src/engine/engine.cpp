#include "engine/ui.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <raylib.h>
#include <raymath.h>
#include <engine/engine.hpp>
#include <engine/entities.hpp>
#include <utils/slotmap.hpp>

constexpr int unused = 65535;

Player::Player(Slot hq_key) : units(50), buildings(50){
	buildings.emplace_back(hq_key);
}

namespace engine {

Game::Game(Camera2D &camera) : camera(camera), units(50), buildings(10), ui_manager(camera){
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

	//CHANGE PLACEHOLDER
	int row = 7;
	int iter = 0;
	while(players.size() < playerCount) {
		HexSpace *hex = &this->grid_space[0][players.size() * row];
		Building hq(hex, HQ, this->players.size());
		
		Slot hq_key = this->buildings.Insert(hq);
		players.emplace_back(Player(hq_key));
		iter++;
	}
}

//ADD STATE CHANGES
void Game::endTurn(){
	std::cout << "End Turn!" << std::endl;

	this->escape();

	std::vector<Slot> &player_units = this->players[player_index].units;
	for (size_t i = 0; Slot unit_key: player_units){
		std::optional<Unit&> unit = this->units[unit_key];
		if(unit){
			unit->atks_left = 1; //change this to be its default value;
		} else {
			player_units.erase(player_units.begin() + i);
		}
		++i;
	}

	player_index = (player_index + 1) % player_count;
	//reset units to have their default attack amount 
	
	std::cout << "Player Index: " << player_index << std::endl;
	//state transition

	return;
}

engine::states Game::SelectBuilding(Building *building_ptr){
	this->escape();
	this->MousePosition = GetScreenToWorld2D(GetMousePosition(), this->camera);
	this->selected_hex = building_ptr->hex;

	if(building_ptr->owner_index != this->player_index or building_ptr->type != FACTORY){
		this->ui_manager.createUiElem(Vector2(), ui::ElemTypes::kInfo, ui::CommandParams());
		return HEX_INFO;

	} else {
		this->ui_manager.createUiElem(this->MousePosition, ui::ElemTypes::kUnitScroll, ui::CommandParams());
		return kBuildingOpt;
	}
}

engine::states Game::SelectUnit(Unit* unit_ptr){
	this->escape(); //put the engine into an idle state
	bool unit_owned = unit_ptr->owner_index == this->player_index;
	//HEX INFO NEEDS THIS TOO
	if(unit_owned){
		std::cout << "owned" << std::endl;
		this->selected_unit = unit_ptr;
		this->selected_hex = unit_ptr->current_hex;

		std::optional<Slot> building_key = this->selected_hex->structure_key;
		bool exists = this->buildings[building_key].has_value();
		bool owned = (exists) ? this->buildings[*building_key]->owner_index == this->player_index : false;

		if(unit_ptr->task != NONE or (exists and !owned)) {
			Vector2 button_position = unit_ptr->position;
			this->MousePosition = GetScreenToWorld2D(GetMousePosition(), this->camera);
			ui_manager.createUiElem(this->MousePosition, ui::ElemTypes::kTaskScroll, ui::CommandParams());
		}
		return UNIT1;

	} else {
		//SHOW COMPARISON
		this->selected_unit = unit_ptr;
		this->ui_manager.createUiElem(Vector2(), ui::ElemTypes::kInfo, ui::CommandParams());
		return UNIT_INFO;
	}
}


void Game::escape(){
	this->state = IDLE;
	this->selected_unit = nullptr;
	this->selected_unit2 = nullptr;

	this->selected_hex = nullptr;
	this->selected_hex2 = nullptr;

	this->ui_manager.hideElements();
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
		case UNIT:{
			std::cout << "Unit Selected" << std::endl;
			Unit *unit_ptr = (Unit*)selection;

			if(unit_ptr->owner_index != this->player_index){
				this->selected_unit = unit_ptr;
				this->selected_hex = unit_ptr->current_hex;

				this->ui_manager.createUiElem(Vector2(), ui::ElemTypes::kInfo, ui::CommandParams());
				this->state = UNIT_INFO;
				return;
			}

			this->selected_unit = unit_ptr;
			this->selected_hex = unit_ptr->current_hex;
			this->state = UNIT1;
			this->ui_manager.hideElements();
			break;
			  }
		case BUILDING:{
			Building *building_ptr = (Building*)selection;
			this->state = this->SelectBuilding(building_ptr);
			break;
		        }
		default:
			std::cerr << "STATE " << input << " NOT HANDLED (hexInfoTransition)" << std::endl;
			break;
	}
}

void Game::idleTransition(inputAlphabet input, void *selection){
	switch(input){
		case TURNEND:
			break;
		case UNIT: {
			std::cout << "Unit Selected" << std::endl;
			Unit *unit_ptr = (Unit*)selection;

			bool unit_owned = unit_ptr->owner_index == this->player_index;
			this->state = SelectUnit(unit_ptr);
			break;
			   }
		case HEX:
			this->selected_hex = (HexSpace*)selection;

			this->ui_manager.createUiElem(Vector2(), ui::ElemTypes::kInfo, ui::CommandParams());
			this->state = HEX_INFO;
			break;
		case BUILDING:{
			Building *building_ptr = (Building*)selection;
			this->state = this->SelectBuilding(building_ptr);
			break;
			}
		default:
			std::cerr << "ERR: INPUT NOT FOUND (idleTransition)" << std::endl;
			return;

	}
}

//this will handle fire and option transition
void Game::optionTransition(inputAlphabet input, void *selection){
	switch(input){
		case UNIT:{
			Unit* unit_ptr = (Unit*)selection;
			bool eql_unit2;
			if(this->state == FIRE) eql_unit2 = this->selected_unit2 == unit_ptr;
			if(this->selected_unit == unit_ptr or eql_unit2){
				if(this->state == FIRE) this->selected_unit2 = nullptr;
				this->selected_hex2 = nullptr;
				this->state = UNIT1;
				this->escape();
				return;
			} else if (unit_ptr->owner_index == this->player_index){
				//for now change the selected unit
				if (this->state == FIRE) this->selected_unit2 = nullptr;
				this->selected_hex2 = nullptr;
				this->selected_unit = unit_ptr;
				this->selected_hex = unit_ptr->current_hex;
				this->state = UNIT1;
				this->ui_manager.hideElements();
			} else {
				HexSpace* hex_ptr = unit_ptr->current_hex;
				if (std::abs(hex_ptr->indices.x - selected_hex->indices.x) <= selected_unit->attack_range and
				  std::abs(hex_ptr->indices.y - selected_hex->indices.y) <= selected_unit->attack_range){
					this->selected_unit2 = unit_ptr;
					if(this->state != FIRE){
						this->state = FIRE;

						this->MousePosition = GetScreenToWorld2D(GetMousePosition(), this->camera);
						ui::CommandParams params;
						params.fireable = true;

						this->ui_manager.createUiElem(this->MousePosition, ui::ElemTypes::kOptionScroll, params);
					};
				} 
				//otherwise idk 
			}
			break;
			  }
		case HEX:{	
			HexSpace *hex_ptr = (HexSpace*)selection;

			if(hex_ptr == selected_hex2){
				this->escape();
				return;
			}

			if(this->state == FIRE) {
				this->state = OPTIONS;
				this->selected_unit2 = nullptr;
			}

			//what purpose does this serve
			// if(this->buildings[hex_ptr->structure_key].has_value()){ 
			// 	this->ui_manager.hideElements();
			// 	return;
			// };
			this->selected_hex2 = hex_ptr;
			this->MousePosition = GetScreenToWorld2D(GetMousePosition(), this->camera);

			ui::CommandParams params;
			params.movable = true;

			this->ui_manager.createUiElem(this->MousePosition, ui::ElemTypes::kOptionScroll, params);
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
		case HEX:{
			std::cout << "Hex 2 Selected" << std::endl;
			HexSpace* hex_ptr = (HexSpace*) selection;

			if(this->selected_unit->owner_index != this->player_index){
				this->selected_unit = nullptr;
				this->selected_hex = hex_ptr;

				this->ui_manager.createUiElem(Vector2(), ui::ElemTypes::kInfo, ui::CommandParams());
				this->state = HEX_INFO;
				return;
			}

			if(this->buildings[hex_ptr->structure_key].has_value()) return; 

			this->selected_hex2 = hex_ptr;
			this->MousePosition = GetScreenToWorld2D(GetMousePosition(), this->camera);
			this->state = OPTIONS;

			ui::CommandParams params;
			params.movable = true;
			
			this->ui_manager.createUiElem(this->MousePosition, ui::ElemTypes::kOptionScroll, params);
			break;
			 }
		case BUILDING:{
			Building *building_ptr = (Building*)selection;

			this->state = this->SelectBuilding(building_ptr);
			break;
			}
	}
}

void Game::unitTransition(inputAlphabet input, void *selection){
	switch(input){
		case UNIT:{
			Unit *unit_ptr = (Unit*)selection;
			if(unit_ptr == selected_unit) this->escape();

			HexSpace* hex_ptr = unit_ptr->current_hex;
			bool unit_owned = unit_ptr->owner_index == this->player_index;

			if(unit_owned){
			} else if(std::abs(hex_ptr->indices.x - selected_hex->indices.x) <= selected_unit->attack_range and
				  std::abs(hex_ptr->indices.y - selected_hex->indices.y) <= selected_unit->attack_range) {
				this->state = FIRE; //the fire state awaits another input from the handler or signal from uimanager to trigger firing
				
				this->selected_unit2 = unit_ptr;
				this->selected_hex2 = hex_ptr;

				//WORKHRE
				this->MousePosition = {
					.x = selected_unit2->position.x + grid::inradius * 3 / 4,
					.y = selected_unit2->position.y,
				}; //unit position is already world 

				ui::CommandParams params;
				params.fireable = true;
				this->ui_manager.createUiElem(this->MousePosition, ui::ElemTypes::kOptionScroll, params);
				
				//PROB DO FIRE ANIMATION

				std::cout << "FIREEEEEEE" <<std::endl;
			}
			break;
			  }
		case HEX:{
			std::cout << "Hex 2 Selected" << std::endl;
			this->state = OPTIONS;
			HexSpace *hex_ptr = (HexSpace*) selection;
			
			if(this->buildings[hex_ptr->occupier_key].has_value()) return;

			this->MousePosition = GetScreenToWorld2D(GetMousePosition(), this->camera);
			this->selected_hex2 = hex_ptr;

			ui::CommandParams params;

			params.movable = true;
			ui_manager.createUiElem(this->MousePosition, ui::ElemTypes::kOptionScroll, params);
			break;
			 }
		case BUILDING:{
		        //TODO >> CHANGE THIS 
			Building *building_ptr = (Building*)selection;
	//		if(building_ptr->owner_index == )
			break;
		         }
		default:
			return;
	}
}

//scroll should no longer be a state
// void Game::scrollTransition(inputAlphabet input, void *selection){
// 	switch(input){
// 		case UNIT:{
// 			Unit* unit_ptr = (Unit*)selection;
// 			this->state = this->SelectUnit(unit_ptr);
// 			break;
// 			  }
// 		case HEX:
// 			//TODO >> MAKE A HEX HELPER FUNCTION FOR HEXINFO
// 			this->selected_hex = (HexSpace*)selection;
// 			this->state = HEX_INFO;
//
// 			if(this->ui_elements.size() > 1) this->ui_elements.erase(ui_elements.begin() + 1);
// 			ui_manager.createUiElem(UI_INFO);
// 			break;
// 		case BUILDING:{
// 			Building* building_ptr = (Building*)selection;
// 			//ifbuilding not owned pull up hex info
// 			//Check building type to determine if a research or unit scroll menu should be made
// 			this->state = this->SelectBuilding(building_ptr);
// 			break;
// 		      }
// 		default:
// 			return;
// 	}
// }

void Game::transitionState(inputAlphabet input, void *selection){
	std::cout << "CURRENT STATE: " << this->state <<  ", INPUT: " << input << std::endl;
	switch(this->state){
		case IDLE:
			idleTransition(input, selection);
			break;
		case UNIT1:
			unitTransition(input, selection);
			break;
		case FIRE:
		case OPTIONS:
			optionTransition(input, selection);
			break;
		case HEX_INFO:
			hexInfoTransition(input, selection);
			break;
		case UNIT_INFO:
			unitInfoTransition(input, selection);
			break;
		case kBuildingOpt:
			//scrollTransition(input, selection);
			break;
		default:
			std::cerr << "ERR STATE " << input << " NOT FOUND (transitionState)" << std::endl;
			return;
	}
}

void Game::handleCollision(HexSpace *collided_hex, Vector2 mouse_point){
	//add hovering
	if(this->units[collided_hex->occupier_key].has_value() ){
		//CHANGE ALL TO RELEASED OR DOWN (THEY MUST ALL BE THE SAME)
		Unit* unit_ptr = &this->units[collided_hex->occupier_key].value();
		
		if (CheckCollisionPointRec(mouse_point, unit_ptr->render_rect) and IsMouseButtonReleased(0)) {
			//do unit stuff
			std::cout << "Unit Collision Detected" << std::endl;
			this->transitionState(UNIT, unit_ptr);
		} else if(IsMouseButtonReleased(0)) {
			this->transitionState(HEX, collided_hex);
		}
	} else if(this->buildings[collided_hex->structure_key].has_value()){
		//check for building collision
		Building* building_ptr = &this->buildings[collided_hex->structure_key].value();

		if(CheckCollisionPointRec(mouse_point, building_ptr->render_rect) and IsMouseButtonReleased(0)){
			std::cout << "BUILDING DETECTED" << std::endl;
			this->transitionState(BUILDING, building_ptr);
		} else if(IsMouseButtonReleased(0)) {
			this->transitionState(HEX, collided_hex);
		}

	} else {
		if (IsMouseButtonReleased(0)) this->transitionState(HEX, collided_hex);
	}
}
//TODO >> DESTROY THIS FUNCTION
// void Game::scrollCollision(int index, scroll_type type){
// 	switch(type){
// 		case SCRLL_UNITS:
// 			switch(index) {
// 				case 0:{
// 					//SPAWN INFANTRY
// 					std::cout << "PLAYER INDEX: " << this->player_index << std::endl;
// 					Slot key = this->units.Insert(Unit(this->selected_hex,INFANTRY,this->player_index));
//
// 					this->selected_hex->occupier_key = key;
// 					this->players[player_index].units.push_back(key);
// 					//WORKHERE SUBTRACT MONEY AFTER
// 				       }
// 				default:
// 					return;
// 			}
// 		case SCRLL_UPGRADES:
// 			switch (index) {
// 				default:
// 					return;
//
// 			}
// 	}
// }
//

//aidan optimize search (I THINK BINARY SEARCH WILL SHINE HERE)
bool Game::collisionCheck(){
	Vector2 mouse_point = GetScreenToWorld2D(GetMousePosition(), this->camera);
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

//ui_manaager FLAGS/SIGNALS:
//endturn
//move
//fire
//TASK (capture)
//ScrollCollision flag (SPAWN UNIT, RESEARCH UPRADE)

void Game::spawnUnit(ui::UiSignal signal){
	switch(signal){
		case ui::UiSignal::kSigSpawnInfantry:{
			std::cout << "PLAYER INDEX: " << this->player_index << std::endl;
			Slot key = this->units.Insert(Unit(this->selected_hex,INFANTRY,this->player_index));

			this->selected_hex->occupier_key = key;
			this->players[player_index].units.push_back(key);
			//WORKHERE SUBTRACT MONEY AFTER
			break;
		           }
		default:{
			std::cout << "PLAYER INDEX: " << this->player_index << std::endl;
			Slot key = this->units.Insert(Unit(this->selected_hex,INFANTRY,this->player_index));

			this->selected_hex->occupier_key = key;
			this->players[player_index].units.push_back(key);
			break;
			//workhere
			  }
	}
}

void Game::handleSignal(ui::UiSignal signal){
	Vector2 mouse_point = GetMousePosition();
	Vector2 wrld_point = GetScreenToWorld2D(GetMousePosition(), this->camera);
	//0 endturn button, 1 Others(????), 
	//may need to change the way this works to switch on states instead of ui elements
	switch(signal){
		//end turn
		//this is for moving for now
		case ui::UiSignal::kSigMove:
			selected_unit->position.x = selected_hex2->x_position;
			selected_unit->position.y = selected_hex2->y_position;
			selected_unit->current_hex = selected_hex2;

			this->selected_hex2->occupier_key = selected_hex->occupier_key;
			this->selected_hex->occupier_key = std::nullopt;

			this->ui_manager.hideElements();
			this->state = MOVING;
			break;
		case ui::UiSignal::kSigFire:{
			assert("Number of atks Greater than 0" && this->selected_unit->atks_left > 0);
			this->selected_unit->atks_left -= 1;

			float dmg_taken = calcDamage();
			this->selected_unit2->hp -= this->dmg_taken;

			ui::CommandParams params;
			params.text_content = TextFormat("-%.2f HP", dmg_taken);

			Vector2 text_position = {
				.x = this->selected_unit2->position.x - grid::inradius / 2,
				.y = this->selected_unit2->position.y -	grid::radius
			};

			this->ui_manager.createUiElem(text_position,ui::ElemTypes::kFiringText, params);
			this->Fire();
			break;
		    	    }
		case ui::UiSignal::kSigCapture :{
			//a task is being performed or is available we can do this with a scroll bar right now the only task is capture
			this->selected_unit->task = CAPTURING;
			std::optional<Building&> enemy_building = this->buildings[selected_hex->structure_key]; //URGENT check for nullopt

			std::cout << "CAPTURING" << std::endl;
			enemy_building->hp -= 40.0;

			if(enemy_building->hp <= 0){
				this->selected_unit->task = NONE;
				this->transferBuilding(*this->selected_hex->structure_key, enemy_building->owner_index, this->selected_unit->owner_index);
				enemy_building->hp = 100.0;
				enemy_building->owner_index = this->player_index;
			}
			this->escape();
			break;
			   }	
		case ui::UiSignal::kSigSpawnInfantry :{
		        //spawn unit
			this->spawnUnit(signal);
			this->escape();
			break;
		    }
		case ui::UiSignal::kSigEndTurn :{
			this->endTurn();
			break;
						}
		default:
			break;
			//std::cerr << "UI element " << i << " Not Recognized" << std::endl;
	}
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

	//hp
	float i = 1;
	while(i * 10 < this->selected_unit->hp ){
		i++;
	}

	multipier *= (i * .10);

	//apply environmental defense
	if(this->selected_hex2->env_defense > 0){
		float env_mul = .05 * this->selected_hex2->env_defense;
		multipier *= env_mul;
	}

	dmg *= multipier;
	return dmg;
}

//Change this to slotmap
void Game::eraseUnit(const Slot &key){
	this->units.erase(key);
}

//this helper function might not be needed
//the building key should have been checked before this function call so we know the key is not null
void Game::transferBuilding(Slot &building_key, int current_owner, int new_owner){
	std::vector<Slot> &owner_buildings = this->players[player_index].buildings;
	this->buildings[building_key]->owner_index = new_owner;

	for(size_t i = 0; Slot building : owner_buildings){
		if(building_key == building){
			owner_buildings.erase(owner_buildings.begin() + i);
		}
		++i;
	}
	this->players[new_owner].buildings.push_back(building_key);
}

//moves units
void Game::Move(){
	Unit *unit_ptr = this->selected_unit;

	Vector2 destRect = {
		.x = unit_ptr->position.x - grid::inradius / 2,
		.y = unit_ptr->position.y - grid::radius / 2,
	};

	if(destRect.x != unit_ptr->render_rect.x or destRect.y != unit_ptr->render_rect.y){

		Vector2 renderRect = {
			.x = unit_ptr->render_rect.x, 
			.y = unit_ptr->render_rect.y
		};
		float max_dist = GetFrameTime() * 240;
		Vector2 new_pos = Vector2MoveTowards(renderRect, destRect, max_dist);

		unit_ptr->render_rect.x = new_pos.x;
		unit_ptr->render_rect.y = new_pos.y;
	} else {
		escape();
	}
}

void Game::Fire(){
	if(this->selected_unit2->hp < 0){
		this->eraseUnit(*this->selected_hex2->occupier_key); //URGENT CHECK OPTIONAL: this should invalidate the key on hex 2 
	};
	this->escape();
	//edit color according to closeness to the y destination
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

	switch(this->state){
		case MOVING:
			this->Move();
			return;
		case FIRING:
			this->Fire();
			return;
		default:
			break;
	}

	ui::UiSignal ui_signal = this->ui_manager.CollisionCheck();
	
	if(ui_signal == ui::UiSignal::kSigNone) {
		bool chng_state = this->collisionCheck();
	} else {
		this->handleSignal(ui_signal);
	}
	//check_hexagon
}

}
