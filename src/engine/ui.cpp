#include <engine/ui.hpp> 
#include <raylib.h>
#include <iostream>

namespace ui{
	//THIS SHOULD BE CHANGED FROM RENDER OPTIONS TO RENDER UI
void renderOptions(const engine::Game &engine_instance, std::unordered_map<int, Texture2D> texture_map){
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
			UnitScrollMenu *scrl_menu = dynamic_cast<UnitScrollMenu*>(engine_instance.scrl_menu.get());

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

			const std::vector<Rectangle> &elements = scrl_menu->elements;

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
