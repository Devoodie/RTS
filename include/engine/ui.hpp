#ifndef RTS_UI_H
#define RTS_UI_H

#include <unordered_map>
#include <cstdint>
#include <raylib.h>
#include <engine/engine.hpp>

//contains ui rendering functions
namespace ui {
	//renders options menu
	void renderOptions(const engine::Game &engine_instance, std::unordered_map<int, Texture2D> texture_map);
	void renderText(const std::vector<Text> &messages);

	class Element {
		Rectangle renderRect;


		Element();
	};
}
#endif
