#include <engine/ui.hpp> 
#include <raylib.h>

namespace ui{

	ScrollMenu::ScrollMenu(scroll_type menu_type, Rectangle placement) : type(menu_type), dimensions(placement) {
		switch(menu_type){
			case UNITS:
				this->y_pos = 0;
				for(int i = 0; i < 3; ++i){
					this->elements.emplace_back((Rectangle){
						.x = 0,
						.y = i * grid::radius / 2,
						.width  = (float)grid::inradius * 4,
						.height = grid::radius / 2,
					});
				}
				break;
			case UPGRADES:
				break;
		}

	};

	//THIS SHOULD BE CHANGED FROM RENDER OPTIONS TO RENDER UI
	void renderOptions(const engine::Game &engine_instance, std::unordered_map<int, Texture2D> texture_map){
		switch(engine_instance.state){
			case engine::FIRE:
			case engine::OPTIONS:{

				BeginMode2D(engine_instance.camera);
				Texture2D opt_texture;
				if(engine_instance.state == engine::FIRE){
					opt_texture = texture_map[grid::FIRE_BUTTON];
				} else{
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
					      //Get a bunch of shit
					      break;
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
