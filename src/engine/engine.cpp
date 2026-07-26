#include <cstdlib>
#include <iostream>
#include <cassert>
#include <raylib.h>
#include <raymath.h>
#include <engine/engine.hpp>
#include <engine/entities.hpp>

constexpr int unused = 65535;

Player::Player(int hq_index){
	units = std::vector<uint16_t>(12);
	buildings = std::vector<uint8_t>(12);
	buildings.emplace_back(hq_index);
}

ScrollMenu::ScrollMenu(scroll_type menu_type, const Vector2 &mouse_position) : type(menu_type) {	
	switch(menu_type){
		case SCRLL_UNITS:
			this->y_pos = 0;
			for(int i = 0; i < 3; ++i){
				this->elements.emplace_back((Rectangle){
					.x = 0,
					.y = i * grid::radius / 2,
					.width  = (float)grid::inradius * 4,
					.height = grid::radius / 2,
				});
			}
			this->internal_height = elements[2].y + elements[2].height;
			break;
		case SCRLL_UPGRADES:
			break;
	}
	float height;
	//max height is 2 Hex's or 8 elements
	if(elements.size() > 8){
		height = grid::radius / 2 * 8;
	} else {
		height = grid::radius / 2 * elements.size();
	}

	this->dimensions = {
		.x = mouse_position.x + grid::inradius / 4,
		.y = mouse_position.y,
		.width = grid::inradius * 4,
		.height = height,
	};
};

namespace engine {

Game::Game(Camera2D &camera) : camera(camera), scrl_menu(ScrollMenu(SCRLL_UNITS, {0})) {
	players = std::vector<Player>();
	player_count = 2;
	player_index = 0;
	grid_space = std::vector<std::vector<HexSpace>>(8, std::vector<HexSpace>(8));
	units.reserve(50);
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
		this->buildings.emplace_back(hex, HQ, this->players.size());

		hex->structure_index = iter;
		players.emplace_back(Player(iter));
		iter++;
	}
}

//ADD STATE CHANGES
void Game::endTurn(){
	std::cout << "End Turn!" << std::endl;

	this->escape();

	for (uint16_t unit_index : this->players[player_index].units){
		Unit &unit = this->units[unit_index];
		unit.atks_left = 1;
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

	if(building_ptr->owner_index != this->player_index){
		this->createUiElem(UI_INFO);
		return HEX_INFO;

	} else {
		this->createUiElem(UNIT_SCRL);
		return SCROLL;
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
		return UNIT1;

	} else {
		//SHOW COMPARISON
		this-> selected_unit = unit_ptr;
		this->createUiElem(UI_INFO);
		return UNIT_INFO;
	}
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
				.x = float((grid::ScreenWidth * 3) / 4.0 ),
				.y = 0,
				.width = float(grid::ScreenWidth / 4.0),
				.height = float(grid::ScreenHeight),
				});
			break;
			//temporary
		case UI_FIRING_TEXT:
			{
			this->dmg_txt_index = this->messages.size();
			Vector2 text_pos = {
				.x = this->selected_unit2->position.x - grid::inradius / 2,
				.y = this->selected_unit2->position.y - grid::radius,
			};

			this->messages.emplace_back((Text){
					.content = TextFormat("-%.2f HP", this->dmg_taken),
					.text_color = RED,
					.position = text_pos,
					.font_size = 15,
					});
			break;
			}
		case UNIT_SCRL:
			// CREATE NEW SCROLL
			this->scrl_menu = ScrollMenu(SCRLL_UNITS, this->MousePosition);
			this->ui_elements.push_back(scrl_menu.dimensions);
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

			if(unit_ptr->owner_index != this->player_index){
				this->selected_unit = unit_ptr;
				this->selected_hex = unit_ptr->current_hex;
				this->state = UNIT_INFO;
				return;
			}

			this->selected_unit = unit_ptr;
			this->selected_hex = unit_ptr->current_hex;
			this->state = UNIT1;
			if(this->ui_elements.size() > 1) this->ui_elements.erase(ui_elements.begin() + 1);
			break;

		defaut:
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
			this->state = HEX_INFO;
			this->createUiElem(UI_INFO);
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
				this->ui_elements.erase(ui_elements.begin() + 1);
				this->escape();
				return;
			} else if (unit_ptr->owner_index == this->player_index){
				//for now change the selected unit
				if (this->state == FIRE) this->selected_unit2 = nullptr;
				this->selected_hex2 = nullptr;
				this->selected_unit = unit_ptr;
				this->selected_hex = unit_ptr->current_hex;
				this->state = UNIT1;
				this->ui_elements.erase(ui_elements.begin() + 1);
			} else {
				HexSpace* hex_ptr = unit_ptr->current_hex;
				if (std::abs(hex_ptr->indices.x - selected_hex->indices.x) <= selected_unit->attack_range and
				  std::abs(hex_ptr->indices.y - selected_hex->indices.y) <= selected_unit->attack_range){
					this->selected_unit2 = unit_ptr;
					if(this->state != FIRE){
						this->state = FIRE;
						this->ui_elements[1].x = unit_ptr->position.x;
						this->ui_elements[1].y = unit_ptr->position.y;
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

			if(hex_ptr->occupier_index != unused){ 
				this->ui_elements[1].x = 16384;
				this->ui_elements[1].y = 16384;
				return;
			};
			this->selected_hex2 = hex_ptr;
			this->MousePosition =  GetScreenToWorld2D(GetMousePosition(), this->camera);

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
		case HEX:{
			std::cout << "Hex 2 Selected" << std::endl;
			HexSpace* hex_ptr = (HexSpace*) selection;

			if(this->selected_unit->owner_index != this->player_index){
				this->selected_unit = nullptr;
				this->selected_hex = hex_ptr;
				this->state = HEX_INFO;
				return;
			}

			if(hex_ptr->occupier_index != unused) return; 

			this->selected_hex2 = hex_ptr;
			this->MousePosition = GetScreenToWorld2D(GetMousePosition(), this->camera);
			this->state = OPTIONS;

			if(this->ui_elements.size() > 1) this->ui_elements.erase(ui_elements.begin() + 1);
			this->createUiElem(UI_OPTIONS_1);
			break;
			 }
		case BUILDING:{
			Building *building_ptr = (Building*)selection;

			this->selected_hex = building_ptr->hex;
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

				this->selected_unit2 = unit_ptr;
				this->selected_hex2 = hex_ptr;

				Vector2 button_position = unit_ptr->position;
				this->MousePosition = GetScreenToWorld2D(GetMousePosition(), this->camera);
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
			
			if(hex_ptr->occupier_index != unused) return;

			this->MousePosition = GetScreenToWorld2D(GetMousePosition(), this->camera);
			this->selected_hex2 = hex_ptr;
			this->state = OPTIONS;

			//create ui element for options
			this->createUiElem(UI_OPTIONS_1);
			break;
			 }
		case BUILDING:{
		        //TODO >> CHANGE THIS 
		         }
		default:
			return;
	}
}

void Game::scrollTransition(inputAlphabet input, void *selection){
	switch(input){
		case UNIT:{
			Unit* unit_ptr = (Unit*)selection;
			this->state = this->SelectUnit(unit_ptr);
			break;
			  }
		case HEX:
			//TODO >> MAKE A HEX HELPER FUNCTION FOR HEXINFO
			this->selected_hex = (HexSpace*)selection;
			this->state = HEX_INFO;

			if(this->ui_elements.size() > 1) this->ui_elements.erase(ui_elements.begin() + 1);
			this->createUiElem(UI_INFO);
			break;
		case BUILDING:{
			Building* building_ptr = (Building*)selection;
			//ifbuilding not owned pull up hex info
			//Check building type to determine if a research or unit scroll menu should be made
			this->state = this->SelectBuilding(building_ptr);
			break;
		      }
		default:
			return;
	}
}

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
		case SCROLL:
			scrollTransition(input, selection);
			break;
		default:
			std::cerr << "ERR STATE " << input << " NOT FOUND (transitionState)" << std::endl;
			return;
	}
}

void Game::handleCollision(HexSpace *collided_hex, Vector2 mouse_point){
	//add hovering
	if(collided_hex->occupier_index != unused){
		//CHANGE ALL TO RELEASED OR DOWN (THEY MUST ALL BE THE SAME)
		Unit* unit_ptr = &this->units[collided_hex->occupier_index];
		
		if (CheckCollisionPointRec(mouse_point, unit_ptr->render_rect) and IsMouseButtonReleased(0)) {
			//do unit stuff
			std::cout << "Unit Collision Detected" << std::endl;
			this->transitionState(UNIT, unit_ptr);
		} else if(IsMouseButtonReleased(0)) {
			this->transitionState(HEX, collided_hex);
		}
	} else if(collided_hex->structure_index != unused){
		//check for building collision
		Building* building_ptr = &this->buildings[collided_hex->structure_index];

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

void Game::scrollCollision(int index, scroll_type type){
	switch(type){
		case SCRLL_UNITS:
			switch(index) {
				case 0:
					//SPAWN INFANTRY
					std::cout << "PLAYER INDEX: " << this->player_index << std::endl;
					this->selected_hex->occupier_index = this->units.size();
					this->players[player_index].units.push_back(this->units.size());
					this->units.emplace_back(
							this->selected_hex,
							INFANTRY,
							this->player_index
							);
					//WORKHERE SUBTRACT MONEY AFTER
				default:
					return;
			}
		case SCRLL_UPGRADES:
			switch (index) {
				default:
					return;
				
			}
	}
}

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

bool Game::uiCollisionCheck(){
	Vector2 mouse_point = GetMousePosition();
	Vector2 wrld_point = GetScreenToWorld2D(GetMousePosition(), this->camera);
	//0 endturn button, 1 Others(????), 
	//may need to change the way this works to switch on states instead of ui elements
	
	if(CheckCollisionPointRec(mouse_point, ui_elements[0]) and IsMouseButtonReleased(0)){
		std::cout << "ENDTURN" << std::endl;
		this->endTurn();
		return true;
	}
	if(this->ui_elements.size() < 2) return false;
	if(CheckCollisionPointRec(wrld_point, ui_elements[1]) and IsMouseButtonReleased(0)){
		switch(this->state){
			//end turn
			case OPTIONS:
					selected_unit->position.x = selected_hex2->x_position;
					selected_unit->position.y = selected_hex2->y_position;
					selected_unit->current_hex = selected_hex2;

					this->selected_hex2->occupier_index = selected_hex->occupier_index;
					this->selected_hex->occupier_index = unused;

					this->state = MOVING;
					std::cout << "MOVE!" << std::endl;
					break;
				
			case FIRE:
					assert("Number of atks Greater than 0" && this->selected_unit->atks_left > 0);
					this->selected_unit->atks_left -= 1;

					this->dmg_taken = calcDamage();
					this->selected_unit2->hp -= this->dmg_taken;

					this->createUiElem(UI_FIRING_TEXT);
					this->state = FIRING;
					break;
			case SCROLL:{
					//do linear search for mouse position within scroll menu
					float target = wrld_point.y - this->scrl_menu.dimensions.y + this->scrl_menu.y_pos;
					int collision_index = 0;
					const std::vector<Rectangle> &elements = this->scrl_menu.elements;
					for(int i = 0; i < elements.size(); ++i){	
						Rectangle collision_rect = elements[i];
						if(target >= collision_rect.y and target <= collision_rect.y + collision_rect.height){
							collision_index = i;
						}

					}
					std::cout << "COLLISION INDEX: " << collision_index << std::endl;
					this->scrollCollision(collision_index, SCRLL_UNITS);
					this->escape();
					break;
				    }
			default:
				//std::cerr << "UI element " << i << " Not Recognized" << std::endl;
				return false;
		}
		return true;
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
void Game::popUnit(uint16_t rm_index){
	//pop
	this->units.erase(units.begin() + rm_index);
	for(uint16_t &unit_index : this->players[player_index].units){
		if(unit_index >= rm_index) unit_index -= 1;
	}

	for(auto &hex_row: this->grid_space){
		for(HexSpace &hex : hex_row){
			if(hex.occupier_index != unused and hex.occupier_index >= rm_index) hex.occupier_index -= 1;
		}
	}
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
	float y_dest = this->selected_unit2->position.y - grid::radius * 2.5;
	Text *firing_text = &this->messages[dmg_txt_index];

	if(firing_text->position.y == y_dest) {
		messages.erase(messages.begin() + dmg_txt_index);
		if(this->selected_unit2->hp < 0){
		// free the unit
			this->selected_unit2 = nullptr;
			this->popUnit(this->selected_hex2->occupier_index);

			this->selected_hex2->occupier_index = unused;
			this->selected_hex2 = nullptr;
		};
		this->escape();
	}
	
	Color &fire_text = firing_text->text_color;

	Vector2 destination = {
		.x = firing_text->position.x,
		.y = y_dest,
	};

	//edit color according to closeness to the y destination

	//NEEDS CHANGES
	float max_dist = GetFrameTime() * 40;
	Vector2 new_coords = Vector2MoveTowards(firing_text->position, destination, max_dist);
	firing_text->position = new_coords;
}

void Game::versus(){
	if(players.size() == 0) playerInit(player_count);


	Player current_player = players[player_index];

	//check collisions
	//uichecks
	//

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

	bool ui_collision = this->uiCollisionCheck();
	
	if(!ui_collision) {
		bool chng_state = this->collisionCheck();
	}
	//check_hexagon
}

}
