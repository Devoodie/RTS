#ifndef RTS_UI_H
#define RTS_UI_H

#include <unordered_map>
#include <raylib.h>
#include <engine/engine.hpp>
#include <utils/slotmap.hpp>

//contains ui rendering functions
namespace ui {

//will provide information to autofill menus on creation
enum ScrollType{
	kScrollUnits,
	kScrollUpgrades,
	kScrollOptions,
};

//i would have to make a signal for each unit to spawn
enum UiSignal{
	kSigEndTurn,
	kSigMove,
	kSigFire,
	kSigCapture,
	kSigSpawnInfantry,
	kSigNone,
};

//element will likely have types in the future that change rendering behavior
class Element {
	public:
	Rectangle render_rect;

	Element(Rectangle rect);
};

class ScrollMenu {
	public:
		std::vector<Element> elements;
		Rectangle dimensions;
		float y_pos;
		float internal_height;

		ScrollType type;


		ScrollMenu(ScrollType menu_type, const Vector2 &mouse_position);
		virtual ~ScrollMenu();

		virtual UiSignal HandleScrollCollision(int collision_index) = 0;
		virtual std::vector<Texture2D> GetTextures(std::unordered_map<int, Texture2D> texture_map);
};

class UnitScrollMenu : public ScrollMenu {
	public: 
		UnitScrollMenu(const Vector2 &mouse_position);
		~UnitScrollMenu();

		UiSignal HandleScrollCollision(int collision_index) override;
		std::vector<Texture2D> GetTextures(std::unordered_map<int, Texture2D> texture_map) override;
};

class OptionScrollMenu: public ScrollMenu {
	public: 
		bool fireable;
		bool moveable;
		bool captureable;

		OptionScrollMenu(const Vector2 &mouse_position);
		~OptionScrollMenu();

		UiSignal HandleScrollCollision(int collision_index) override;
		std::vector<Texture2D> GetTextures(std::unordered_map<int, Texture2D> texture_map) override;
};

class InfoPanel {
	Rectangle render_rect;
	std::vector<Rectangle> elements;
	InfoPanel();
	void renderElements(const engine::Game &engine_instance, std::unordered_map<int, Texture2D> texture_map);
};

class UiManager {
	public:
		std::vector<Element> ui_elements;
		Camera2D &camera;
		std::unique_ptr<ScrollMenu> scrl_menu; // only one scroll menu

		UiManager(Camera2D &camera);
		UiSignal CollisionCheck(engine::states engine_state);

		//renders options menu
		void renderUi(const engine::Game &engine_instance, std::unordered_map<int, Texture2D> texture_map);
		void renderText(const std::vector<Text> &messages);
};
}
#endif
