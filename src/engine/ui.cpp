#include <engine/ui.hpp> 
#include <raylib.h>

namespace ui{
	//THIS SHOULD BE CHANGED FROM RENDER OPTIONS TO RENDER UI
	//CHECK NULL POINTERS TO DETERMINE WHICH RECTANGLES SHOULD BE RENDERED
	void renderOptions(const engine::Game &engine_instance, std::unordered_map<int, Texture2D> texture_map){
		switch(engine_instance.state){
			case engine::OPTIONS:{
				Texture2D &move_button = texture_map[grid::MOVE_BUTTON];

				Rectangle texture_rect = {
				     .x = 0,
				     .y = 0,
				     .width = (float)move_button.width,
				     .height = (float)move_button.height,
				};

			        Rectangle options = engine_instance.ui_elements[1];
			        DrawTexturePro(move_button, texture_rect, options, (Vector2){.x = 0, .y = 0}, 0, RAYWHITE);
				break;
				}
			case engine::FIRE:{
				Texture2D &move_button = texture_map[grid::FIRE_BUTTON];

				Rectangle texture_rect = {
				     .x = 0,
				     .y = 0,
				     .width = (float)move_button.width,
				     .height = (float)move_button.height,
				};

			        Rectangle options = engine_instance.ui_elements[1];
			        DrawTexturePro(move_button, texture_rect, options, (Vector2){.x = 0, .y = 0}, 0, RAYWHITE);
				break;
				  }
			case engine::FIRING:
//					  DrawText(const char *text, int posX, int posY, int fontSize, Color color);
					  break;
			case engine::UNIT_INFO:
			case engine::HEX_INFO:{
				Texture2D &info_rect= texture_map[grid::INFO_RECT];

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
			default:
				break;
		}
	}

}
