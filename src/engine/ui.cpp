#include <engine/ui.hpp> 
#include <raylib.h>
#include <iostream>
#include <engine/engine.hpp>
#include <raymath.h>

namespace ui{
Element::Element(Rectangle rect) : render_rect(rect){
}

ScrollMenu::ScrollMenu(ScrollType menu_type, const Vector2 &mouse_position) : type(menu_type) {	
	this->y_pos = 0;
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

UnitScrollMenu::UnitScrollMenu(const Vector2 &mouse_position) : ScrollMenu(kScrollUnits, mouse_position){
	for(int i = 0; i < 3; ++i){
		this->elements.emplace_back((Rectangle){
			.x = 0,
			.y = i * grid::radius / 2,
			.width  = (float)grid::inradius * 4,
			.height = grid::radius / 2,
		});
	}
	this->internal_height = elements[2].render_rect.y + elements[2].render_rect.height;
} 

UnitScrollMenu::~UnitScrollMenu(){}

//TODO >> Change this when we get assets
std::vector<Texture2D> UnitScrollMenu::GetTextures(std::unordered_map<int, Texture2D> texture_map){
	std::vector<Texture2D> textures(3);
	Texture2D elem_texture;
	textures[0] = texture_map[grid::kMoveButton];
	textures[1] = texture_map[grid::kMoveButton];
	textures[2] = texture_map[grid::kMoveButton];
	return textures;
}

OptionScrollMenu::OptionScrollMenu(const Vector2 &mouse_position): ScrollMenu(ui::ScrollType::kScrollOptions, mouse_position) {
	for(int i = 0; i < 3; ++i){
		this->elements.emplace_back((Rectangle){
				.x = 0,
				.y = i * grid::radius / 2,
				.width = (float)grid::inradius,
				.height = grid::radius / 2
				});
	}
}

OptionScrollMenu::~OptionScrollMenu(){}

UiSignal OptionScrollMenu::HandleScrollCollision(int collision_index){
	switch(collision_index){
		case 0:
			return kSigMove;
		case 1:
			return kSigFire;
		case 3:
			return kSigCapture;
	}
	return kSigNone;
}

std::vector<Texture2D> OptionScrollMenu::GetTextures(std::unordered_map<int, Texture2D> texture_map){
	std::vector<Texture2D> textures(3);
	Texture2D elem_texture;
	textures[0] = ((this->moveable) ? texture_map[grid::kMoveButton] : texture_map[grid::kMoveButtonUnusable]);
	textures[1] = ((this->fireable) ? texture_map[grid::kFireButton] : texture_map[grid::kFireButtonUnusable]);
	textures[2] = ((this->captureable) ? texture_map [grid::kCaptureButton] : texture_map[grid::kCaptureButtonUnusable]);
	return textures;
}

UiSignal UnitScrollMenu::HandleScrollCollision(int collision_index){
	switch(collision_index){
		case 0:
			return kSigSpawnInfantry;
		default:
			return kSigSpawnInfantry;

	}
}

InfoPanel::InfoPanel(){
	this->render_rect = (Rectangle){
//		.x = float((grid::ScreenWidth * 3) / 4.0 ),
		.x = float(grid::ScreenWidth + 1),
		.y = 0,
		.width = float(grid::ScreenWidth / 4.0),
		.height = float(grid::ScreenHeight),
	};
}

void InfoPanel::renderElements(const engine::Game &engine_instance, std::unordered_map<int, Texture2D> texture_map){
	//prob switch on info panel information
	Texture2D &info_texture = texture_map[grid::kInfoRect];

	Rectangle texture_rect = {
	     .x = 0,
	     .y = 0,
	     .width = (float)info_texture.width,
	     .height = (float)info_texture.height,
	};
	DrawTexturePro(info_texture, texture_rect, this->render_rect, (Vector2){.x = 0, .y = 0}, 0, RAYWHITE);
}

Transformation::Transformation(Rectangle *current_pos, Rectangle desired_pos) : position(current_pos), target_pos(desired_pos){};

UiManager::UiManager(Camera2D &camera): camera(camera), scrl_menu(new OptionScrollMenu(Vector2{.x = 65535, .y = 65535})), transformations(10) {
	//endturn button
	this->ui_elements.push_back((Rectangle){
		.x = (float)grid::ScreenWidth * 7 / 8,
		.y = (float)grid::ScreenHeight / 5,
		.width = (float)grid::inradius,
		.height = grid::radius / 2
	});
}

UiSignal UiManager::CollisionCheck(){
	Vector2 mouse_point = GetMousePosition();
	Vector2 wrld_point = GetScreenToWorld2D(GetMousePosition(), this->camera);
	//0 endturn button, 1 Others(????), 
	
	if(CheckCollisionPointRec(mouse_point, ui_elements[0].render_rect) and IsMouseButtonReleased(0)){
		std::cout << "ENDTURN" << std::endl;
		return kSigEndTurn;
	}
//	assert(this->scrl_menu != nullptr && "SCROLL MENU REFERENCED BEFORE ALLOCATED");
	if(this->scrl_menu == nullptr) return kSigNone;

	if(CheckCollisionPointRec(mouse_point, scrl_menu->dimensions)){
		float target = wrld_point.y - this->scrl_menu->dimensions.y + this->scrl_menu->y_pos;
		int collision_index = 0;
		const std::vector<Element> &elements = this->scrl_menu->elements;
		for(int i = 0; i < elements.size(); ++i){	
			Rectangle collision_rect = elements[i].render_rect;
			if(target >= collision_rect.y and target <= collision_rect.y + collision_rect.height){
				collision_index = i;
				break;
			}

		}
		std::cout << "COLLISION INDEX: " << collision_index << std::endl;
		return this->scrl_menu->HandleScrollCollision(collision_index);
	}

	return kSigNone;
}

void UiManager::createUiElem(Vector2 position, ElemTypes type, CommandParams params){
	switch(type){
		case ElemTypes::kOptionScroll:
			{
			if(typeid(*this->scrl_menu) != typeid(OptionScrollMenu)){
				this->scrl_menu = std::make_unique<OptionScrollMenu>(position);
			} else {
				this->scrl_menu->dimensions.x = position.x;
				this->scrl_menu->dimensions.y = position.y;
			}
			std::cout << "SCROLL POSITION: " << position.x << ", " << position.y << std::endl;

			OptionScrollMenu &options = *static_cast<OptionScrollMenu*>(this->scrl_menu.get());

			options.fireable = params.fireable;
			options.moveable = params.movable;
			options.captureable = params.capturable;
			break;
			}
		case ElemTypes::kTaskScroll:
			break;
		case ElemTypes::kUnitScroll:
			this->scrl_menu = std::make_unique<UnitScrollMenu>(position);
			break;
		case ui::ElemTypes::kFiringText:
			// can add logic to fit text into rectangle width
			Rectangle text_pos;
			text_pos.x = position.x;
			text_pos.y = position.y;

			Rectangle desired_pos = text_pos;
			desired_pos.y = desired_pos.y - grid::radius * 2.5;	

			Text firing_text = Text(params.text_content, RED, text_pos, 15);
			int text_ind = this->messages.size();
			this->messages.push_back(firing_text);

			Slot transf_key = this->transformations.Insert(Transformation(&this->messages[text_ind].position, desired_pos));
			this->messages[text_ind].transformation = transf_key;
			transformations[transf_key]->self_key = transf_key;
			break;
	}
}

void UiManager::hideElements(){
	this->scrl_menu->dimensions.x = 65535;

	Rectangle desired_pos = this->info.render_rect;
	desired_pos.x = (float)grid::ScreenWidth + 1.0;
	Slot transf_key = this->transformations.Insert(Transformation(&this->info.render_rect, desired_pos));
	transformations[transf_key]->self_key = transf_key;
};

void UiManager::animate(){
	this->transform();
	//other things like color will take place in this
}

void UiManager::transform(){
	std::vector<Slot> del_list;
	for(Transformation trans: this->transformations.values){
		Vector2 target_pos = {.x = trans.position->x, .y = trans.position->y };
		Vector2 new_pos = Vector2MoveTowards(
				(Vector2){ .x = trans.position->x, .y = trans.position->y }, 
				target_pos,
				250.0 * GetFrameTime()
				);
		trans.position->x = new_pos.x;
		trans.position->y = new_pos.y;

		if(new_pos.x == target_pos.x and new_pos.y == target_pos.y){
			del_list.push_back(trans.self_key);
		}
	}

	for(Slot key : del_list){
		this->transformations.erase(key);
	}
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
				texture_rect.width = selected_texture.width;
				texture_rect.height = selected_texture.height;
				// std::cout << "End Turn Location: X: " << elem.render_rect.x << ", Y: " << elem.render_rect.y << std::endl;
				DrawTexturePro(selected_texture, texture_rect, elem.render_rect, (Vector2){.x = 0, .y = 0}, 0, RAYWHITE);
				break;
			default:
				break;
		}
		++i;
	} 

	//render scroll menu 
	std::vector<Texture2D> scrll_textures = this->scrl_menu->GetTextures(texture_map);

	assert(scrll_textures.size() == this->scrl_menu->elements.size() && "Invalid Texture amount to Elements (Scroll Menu)");

	BeginMode2D(camera);
	Texture2D elem_texture;
	//how the fuck does cpp do this shit  without auto? (need verbosity)
	const auto &scrl_menu = this->scrl_menu.get();

	Rectangle dest_rect = {
	     .x = scrl_menu->dimensions.x,
	     .y = scrl_menu->dimensions.y,
	     .width = (float)elem_texture.width,
	     .height = (float)elem_texture.height,
	};

	Rectangle source_rect = {
		.x = 0,
		.y = 0,
		.width = (float)elem_texture.width,
		.height = (float)elem_texture.height,
	};

	float target_y = scrl_menu->y_pos + scrl_menu->dimensions.height;
	if(target_y > scrl_menu->internal_height) target_y = scrl_menu->internal_height;

	float current_y = scrl_menu->y_pos;
	int elem_index = 0;

	const std::vector<Element> &elements = scrl_menu->elements;

	while(current_y < target_y){
		const Rectangle &element = elements[elem_index].render_rect;

		if(current_y > element.y + element.height){
			elem_index += 1;
			continue;
		}
		elem_texture = scrll_textures[elem_index];

		//ratio of how much texture to draw according to how much of the rectangle is showing
		float draw_amount = (element.y + element.height) - current_y;

		if(current_y + draw_amount > target_y){
			draw_amount = target_y - current_y;
		}

		dest_rect.y = current_y + scrl_menu->dimensions.y;
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

	this->info.renderElements(engine_instance, texture_map);
}

void UiManager::renderText(){
	for(Text message: this->messages){
		DrawTextEx(
			GetFontDefault(), 
			message.content.c_str(), 
			(Vector2){.x = message.position.x, .y = message.position.y}, 
			message.font_size,  //placeholder
			5, 
			message.text_color
			);
	}
}
}
