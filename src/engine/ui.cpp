#include <engine/ui.hpp> 
#include <raylib.h>
#include <iostream>
#include <vector>

namespace ui{

ScrollMenu::ScrollMenu(ScrollType menu_type, const Vector2 &mouse_position) : type(menu_type) {	
	switch(menu_type){
		case ui::kScrollUnits:
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
		case ScrollType::kScrollUpgrades:
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

ScrollMenu::~ScrollMenu(){
}

UnitScrollMenu::UnitScrollMenu(const Vector2 &mouse_position): ScrollMenu(kScrollUnits, mouse_position){
	this->y_pos = 0;
		for(int i = 0; i < 3; ++i){
			this->elements.emplace_back((Rectangle){
				.x = 0,
				.y = i * grid::radius / 2,
				.width  = (float)grid::inradius * 4,
				.height = grid::radius / 2,
			});
	}
} 

UnitScrollMenu::~UnitScrollMenu(){
}

OptionScrollMenu::OptionScrollMenu(const Vector2 &mouse_position){
	this->ui_elements.emplace_back((Rectangle){
		.x = this->MousePosition.x + grid::inradius / 4,
		.y = this->MousePosition.y,
		.width = grid::inradius,
		.height = grid::radius / 2,
		});
};

void UnitScrollMenu::handleScrollCollision(){
	// std::cout << "PLAYER INDEX: " << this->player_index << std::endl;
	// this->selected_hex->occupier_index = this->units.size();
	// this->players[player_index].units.push_back(this->units.size());
	// this->units.emplace_back(
	// 		this->selected_hex,
	// 		INFANTRY,
	// 		this->player_index
	// 		);
	//
}

UiManager::UiManager(Camera2D &camera): camera(camera), scrl_menu(nullptr){
}

UiSignal UiManager::CollisionCheck(engine::states engine_state){
	Vector2 mouse_point = GetMousePosition();
	Vector2 wrld_point = GetScreenToWorld2D(GetMousePosition(), this->camera);
	//0 endturn button, 1 Others(????), 
	//may need to change the way this works to switch on states instead of ui elements
	
	if(CheckCollisionPointRec(mouse_point, ui_elements[0].render_rect) and IsMouseButtonReleased(0)){
		std::cout << "ENDTURN" << std::endl;
		return kSigEndTurn;
	}
	if(this->ui_elements.size() < 2) return kSigNone;
	if(CheckCollisionPointRec(wrld_point, ui_elements[1].render_rect) and IsMouseButtonReleased(0)){
		switch(engine_state){
			//end turn
			//this is for moving for now
			case engine::states::OPTIONS:
				//move all this shit to a move function in engine
				// selected_unit->position.x = selected_hex2->x_position;
				// selected_unit->position.y = selected_hex2->y_position;
				// selected_unit->current_hex = selected_hex2;
				//
				// this->selected_hex2->occupier_key = selected_hex->occupier_key;
				// this->selected_hex->occupier_key = std::nullopt;

				return kSigMove;
				break;
			case engine::states::FIRE:
				//move this shit to a fire signal function
				// assert("Number of atks Greater than 0" && this->selected_unit->atks_left > 0);
				// this->selected_unit->atks_left -= 1;
				//
				// this->dmg_taken = calcDamage();
				// this->selected_unit2->hp -= this->dmg_taken;
				//
				// this->createUiElem(UI_FIRING_TEXT);
				// engine_state = FIRING;
				return kSigFire;
				break;
			case engine::states::UNIT1:{
					   //move this shit to task
				// this->selected_unit->task = CAPTURING;
				// std::optional<Building&> enemy_building = this->buildings[selected_hex->structure_key]; //URGENT check for nullopt
				//
				// std::cout << "CAPTURING" << std::endl;
				// std::cout << this->ui_elements.size() << std::endl;
				// enemy_building->hp -= 40.0;
				//
				// if(enemy_building->hp <= 0){
				// 	this->selected_unit->task = NONE;
				// 	this->transferBuilding(*this->selected_hex->structure_key, enemy_building->owner_index, this->selected_unit->owner_index);
				// 	enemy_building->hp = 100.0;
				// 	enemy_building->owner_index = this->player_index;
				// }
				// this->escape();
				return kSigCapture;
				break;
				   }	
			case engine::states::SCROLL:{
				//do linear search for mouse position within scroll menu
				// assert(this->scrl_menu != nullptr && "SCROLL MENU REFERENCED BEFORE ALLOCATED");
				//
				// float target = wrld_point.y - this->scrl_menu->dimensions.y + this->scrl_menu->y_pos;
				// int collision_index = 0;
				// const std::vector<Rectangle> &elements = this->scrl_menu->elements;
				// for(int i = 0; i < elements.size(); ++i){	
				// 	Rectangle collision_rect = elements[i];
				// 	if(target >= collision_rect.y and target <= collision_rect.y + collision_rect.height){
				// 		collision_index = i;
				// 		break;
				// 	}
				//
				// }
				// std::cout << "COLLISION INDEX: " << collision_index << std::endl;
				// this->scrollCollision(collision_index, SCRLL_UNITS);
				// this->escape();
				return kSigSpawn;
				break;
			    }
			default:
				//std::cerr << "UI element " << i << " Not Recognized" << std::endl;
				return kSigNone;
		}
	}
	return kSigNone;
}
	//THIS SHOULD BE CHANGED FROM RENDER OPTIONS TO RENDER UI
void UiManager::renderUi(const engine::Game &engine_instance, std::unordered_map<int, Texture2D> texture_map){
	switch(engine_instance.state){
		case engine::UNIT1:
		case engine::FIRE:
		case engine::OPTIONS:{
			if(engine_instance.ui_elements.size() <= 1) return;

			BeginMode2D(engine_instance.camera);
			Texture2D opt_texture;
			if(engine_instance.state == engine::FIRE){
				opt_texture = texture_map[grid::FIRE_BUTTON];
			} else {
				opt_texture = texture_map[grid::MOVE_BUTTON];
			}

			Rectangle texture_rect = {
			     .x = 0,
			     .y = 0,
			     .width = (float)opt_texture.width,
			     .height = (float)opt_texture.height,
			};

			Rectangle options = engine_instance.ui_elements[1];
			DrawTexturePro(opt_texture, texture_rect, options, (Vector2){.x = 0, .y = 0}, 0, RAYWHITE);
			EndMode2D();

			break;
			}
		case engine::FIRING:
//					  DrawText(const char *text, int posX, int posY, int fontSize, Color color);
				  break;
		case engine::UNIT_INFO:
		case engine::HEX_INFO:{
			Texture2D &info_rect = texture_map[grid::INFO_RECT];

			Rectangle texture_rect = {
			     .x = 0,
			     .y = 0,
			     .width = (float)info_rect.width,
			     .height = (float)info_rect.height,
			};

			Rectangle options = engine_instance.ui_elements[1];
			DrawTexturePro(info_rect, texture_rect, options, (Vector2){.x = 0, .y = 0}, 0, RAYWHITE);
			break;
			}
		case engine::SCROLL:
			{
				//TODO>> CHANGE THIS SHIT NEOW
			BeginMode2D(engine_instance.camera);
			Texture2D &elem_texture = texture_map[grid::MOVE_BUTTON];
			//how the fuck does cpp do this shit  without auto? (need verbosity)
			const auto &scrl_menu = *this->scrl_menu.get();

			Rectangle dest_rect = {
			     .x = scrl_menu.dimensions.x,
			     .y = scrl_menu.dimensions.y,
			     .width = (float)elem_texture.width,
			     .height = (float)elem_texture.height,
			};

			Rectangle source_rect = {
				.x = 0,
				.y = 0,
				.width = (float)elem_texture.width,
				.height = (float)elem_texture.height,
			};

			float target_y = scrl_menu.y_pos + scrl_menu.dimensions.height;
			if(target_y > scrl_menu.internal_height) target_y = scrl_menu.internal_height;

			float current_y = scrl_menu.y_pos;
			int elem_index = 0;

			const std::vector<Rectangle> &elements = scrl_menu.elements;

			while(current_y < target_y){
				const Rectangle &element = elements[elem_index];

				if(current_y > element.y + element.height){
					elem_index += 1;
					continue;
				}

				//ratio of how much texture to draw according to how much of the rectangle is showing
				float draw_amount = (element.y + element.height) - current_y;

				if(current_y + draw_amount > target_y){
					draw_amount = target_y - current_y;
				}

				dest_rect.y = current_y + scrl_menu.dimensions.y;
				dest_rect.height = draw_amount;

				source_rect.y = elem_texture.height = draw_amount;
				source_rect.height = draw_amount;

				DrawTexturePro(
						elem_texture,
						source_rect, 
						dest_rect, 
						(Vector2){.x = 0, .y = 0}, 
						0, 
						RAYWHITE
						);

				current_y += draw_amount;
				elem_index += 1;
			}
			EndMode2D();
				      //Get a bunch of shit
			break;
			}
		default:
			break;
	}
}

void renderText(const std::vector<Text> &messages){
	for(Text message: messages){
		DrawTextEx(
			GetFontDefault(), 
			message.content.c_str(), 
			message.position, 
			message.font_size,  //placeholder
			5, 
			message.text_color
			);
	}
}
}
