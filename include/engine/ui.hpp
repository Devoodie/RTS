#ifndef RTS_UI_H
#define RTS_UI_H

#include <memory>
#include <unordered_map>
#include <raylib.h>
#include <utils/slotmap.hpp>
#include <utils/grid.hpp>

namespace engine {
	class Game;
}
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

struct Text {
	std::string content;
	Color text_color;
	Vector2 position;
	float font_size;
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
//shows when engine state is in UNIT_INFO or HEX_INFO 
class InfoPanel {
	public:
		Rectangle render_rect;
		std::vector<Rectangle> elements;
		InfoPanel();
		void renderElements(const engine::Game &engine_instance, std::unordered_map<int, Texture2D> texture_map);
};

enum class ElemTypes {
	kUnitScroll = 0,
	kOptionScroll,
	kTaskScroll,
	// kInfo,
};

struct CommandParams {
	bool fireable = false;
	bool movable = false;
	bool capturable = false;
};

class UiManager {
	public:
		std::vector<Element> ui_elements;
		Camera2D &camera;
		std::unique_ptr<ScrollMenu> scrl_menu; // only one scroll menu
		InfoPanel info;

		UiManager(Camera2D &camera);
		UiSignal CollisionCheck();

		//overwrites scroll menus (Possibly info panels in the future)
		void createUiElem(Vector2 position, ElemTypes type, CommandParams params);

		//renders options menu
		void renderUi(const engine::Game &engine_instance, std::unordered_map<int, Texture2D> texture_map);
		void renderText(const std::vector<Text> &messages);
};
}
#endif
