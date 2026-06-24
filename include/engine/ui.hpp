#ifndef RTS_UI_H
#define RTS_UI_H

#include <unordered_map>
#include <cstdint>
#include <raylib.h>
#include <engine/engine.hpp>

namespace ui {
	//will provide information to autofill menus on creation
	enum scroll_type {
		UNITS,
		UPGRADES,
	};

	class ScrollMenu {
		std::vector<Rectangle> elements;
		Rectangle dimensions;
		uint16_t y_pos;

		scroll_type type;
		ScrollMenu(scroll_type menu_type, Rectangle placement);
	};

	//renders options menu
	void renderOptions(const engine::Game &engine_instance, std::unordered_map<int, Texture2D> texture_map);
	void renderText(const std::vector<Text> &messages);

	class Element {
		Rectangle renderRect;


		Element();
	};
}
#endif
