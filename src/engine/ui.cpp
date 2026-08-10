#include <engine/ui.hpp> 
#include <raylib.h>
#include <iostream>
#include <vector>

namespace ui{

ScrollMenu::ScrollMenu(ScrollType menu_type, const Vector2 &mouse_position) : type(menu_type) {	
	this-> y_pos = 0;
	float width = 0.0;
	switch(menu_type){
		case ui::kScrollUnits:
			width = grid::inradius * 4.0;
			break;
		case ScrollType::kScrollUpgrades:
			break;
		case ScrollType::kScrollOptions:
			width = grid::inradius;
			break;
	}

	//max height is 8 elements
	float height = 0.0;
	if(elements.size() > 8){
		height = grid::radius / 2 * 8;
	} else {
		height = grid::radius / 2 * elements.size();
	}

	this->dimensions = {
		.x = mouse_position.x + grid::inradius / 4,
		.y = mouse_position.y,
		.width = width,
		.height = height,
	};
};

ScrollMenu::~ScrollMenu(){
}

UnitScrollMenu::UnitScrollMenu(const Vector2 &mouse_position): ScrollMenu(kScrollUnits, mouse_position){
	for(int i = 0; i < 3; ++i){
		this->elements.emplace_back((Rectangle){
			.x = 0,
			.y = i * grid::radius / 2,
			.width  = (float)grid::inradius * 4,
			.height = grid::radius / 2,
		});
	}
	this->internal_height = elements[2].y + elements[2].height;
} 

UnitScrollMenu::~UnitScrollMenu(){
}

//TODO >> Change this when we get assets
void UnitScrollMenu::renderElements(Camera2D &camera, std::unordered_map<int, Texture2D> texture_map){
	BeginMode2D(camera);
	Texture2D &elem_texture = texture_map[grid::kMoveButton];
	//how the fuck does cpp do this shit  without auto? (need verbosity)

	Rectangle dest_rect = {
	     .x = this->dimensions.x,
	     .y = this->dimensions.y,
	     .width = (float)elem_texture.width,
	     .height = (float)elem_texture.height,
	};

	Rectangle source_rect = {
		.x = 0,
		.y = 0,
		.width = (float)elem_texture.width,
		.height = (float)elem_texture.height,
	};

	float target_y = this->y_pos + this->dimensions.height;
	if(target_y > this->internal_height) target_y = this->internal_height;

	float current_y = this->y_pos;
	int elem_index = 0;

	const std::vector<Rectangle> &elements = this->elements;

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

		dest_rect.y = current_y + this->dimensions.y;
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
}

OptionScrollMenu::OptionScrollMenu(const Vector2 &mouse_position): ScrollMenu(kScrollOptions, mouse_position){
	for(int i = 0; i < 3; ++i){
		this->elements.emplace_back((Rectangle){
				.x = 0,
				.y = i * grid::radius / 2,
				.width = (float)grid::inradius,
				.height = grid::radius / 2
				});
	}
};

UiSignal OptionScrollMenu::HandleScrollCollision(int collision_index){
	switch(collision_index){
		case 0:
			return kSigMove;
		case 1:
			return kSigFire;
		case 3:
			return kSigCapture;
	}
}

void OptionScrollMenu::renderElements(Camera2D &camera, std::unordered_map<int, Texture2D> texture_map){
	BeginMode2D(camera);
	Texture2D elem_texture;
	//how the fuck does cpp do this shit  without auto? (need verbosity)
	Rectangle dest_rect = {
	     .x = this->dimensions.x,
	     .y = this->dimensions.y,
	     .width = (float)elem_texture.width,
	     .height = (float)elem_texture.height,
	};

	Rectangle source_rect = {
		.x = 0,
		.y = 0,
		.width = (float)elem_texture.width,
		.height = (float)elem_texture.height,
	};

	float target_y = this->y_pos + this->dimensions.height;
	if(target_y > this->internal_height) target_y = this->internal_height;

	float current_y = this->y_pos;
	int elem_index = 0;

	const std::vector<Rectangle> &elements = this->elements;

	while(current_y < target_y){
		const Rectangle &element = elements[elem_index];

		if(current_y > element.y + element.height){
			elem_index += 1;
			continue;
		}
		switch(elem_index){
			case 0:
				elem_texture = (this->moveable) ? texture_map[grid::kMoveButton] : texture_map[grid::kMoveButtonUnusable];
				break;
			case 1:
				elem_texture = (this->fireable) ? texture_map[grid::kFireButton] : texture_map[grid::kFireButtonUnusable];
				break;
			case 2:
				elem_texture = (this->captureable) ? texture_map [grid::kCaptureButton] : texture_map[grid::kCaptureButtonUnusable];
				break;
		};

		//ratio of how much texture to draw according to how much of the rectangle is showing
		float draw_amount = (element.y + element.height) - current_y;

		if(current_y + draw_amount > target_y){
			draw_amount = target_y - current_y;
		}

		dest_rect.y = current_y + this->dimensions.y;
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
}

UiSignal UnitScrollMenu::HandleScrollCollision(int collision_index){
	switch(collision_index){
		case 0:
			return kSigSpawnInfantry;
		default:
			return kSigSpawnInfantry;

	}
}

UiManager::UiManager(Camera2D &camera): camera(camera), scrl_menu(nullptr){
}

UiSignal UiManager::CollisionCheck(engine::states engine_state){
	Vector2 mouse_point = GetMousePosition();
	Vector2 wrld_point = GetScreenToWorld2D(GetMousePosition(), this->camera);
	//0 endturn button, 1 Others(????), 
	
	if(CheckCollisionPointRec(mouse_point, ui_elements[0].render_rect) and IsMouseButtonReleased(0)){
		std::cout << "ENDTURN" << std::endl;
		return kSigEndTurn;
	}
//	assert(this->scrl_menu != nullptr && "SCROLL MENU REFERENCED BEFORE ALLOCATED");
	if(this->scrl_menu == nullptr) return kSigNone;

	float target = wrld_point.y - this->scrl_menu->dimensions.y + this->scrl_menu->y_pos;
	int collision_index = 0;
	const std::vector<Rectangle> &elements = this->scrl_menu->elements;
	for(int i = 0; i < elements.size(); ++i){	
		Rectangle collision_rect = elements[i];
		if(target >= collision_rect.y and target <= collision_rect.y + collision_rect.height){
			collision_index = i;
			break;
		}

	}
	std::cout << "COLLISION INDEX: " << collision_index << std::endl;
	return this->scrl_menu->HandleScrollCollision(collision_index);

	return kSigNone;
}

//MAKE THIS RENDER EVERYTHING
void UiManager::renderUi(const engine::Game &engine_instance, std::unordered_map<int, Texture2D> texture_map){
	Texture2D selected_texture;

	Rectangle texture_rect = {
	     .x = 0,
	     .y = 0,
	     .width = 0,
	     .height = 0,
	};
	//render static elements
	for(int i = 0; Element elem : this->ui_elements){
		switch(i){
			case 0: 
				selected_texture = texture_map[grid::kEndButton];
				DrawTexturePro(selected_texture, texture_rect, elem.render_rect, (Vector2){.x = 0, .y = 0}, 0, RAYWHITE);
				break;
			default:
				break;
		}
		++i;
	} 
	if(this->scrl_menu == nullptr)
	//render scroll menu 
	this->scrl_menu->renderElements(this->camera, texture_map);

	switch(engine_instance.state){
		case engine::UNIT_INFO:
		case engine::HEX_INFO:{
			Texture2D &info_rect = texture_map[grid::kInfoRect];

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
			Texture2D &elem_texture = texture_map[grid::kMoveButton];
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
