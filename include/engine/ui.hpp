#ifndef RTS_UI_H
#define RTS_UI_H

#include <unordered_map>
#include <raylib.h>
#include <engine/engine.hpp>

//contains ui rendering functions
namespace ui {

enum UiSignal{
	kSigEndTurn,
	kSigMove,
	kSigFire,
	kSigCapture,
	kSigSpawn,
	kSigNone,
};

class UiManager {
	std::vector<Rectangle> ui_elements;
	Camera2D &camera;

	UiManager(Camera2D &camera);
	UiSignal CollisionCheck(engine::states engine_state);
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
