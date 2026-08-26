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
		Texture2D &texture;
		Color color;
		std::optional<Slot> transformation = std::nullopt;

		Element(Rectangle rect, Texture2D& text, Color elem_color);

};

struct Text {
	std::string content;
	Color text_color;
	Rectangle position;
	float font_size;
	std::optional<Slot> transformation = std::nullopt;
	
	Text(std::string message, Color color, Rectangle pos, float size) :  content(message), text_color(color), position(pos), font_size(size){};
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
		virtual std::vector<Texture2D> GetTextures(std::unordered_map<int, Texture2D> texture_map) = 0;
};

class UnitScrollMenu : public ScrollMenu {
	public: 
		UnitScrollMenu(const Vector2 &mouse_position);
		~UnitScrollMenu();

		UiSignal HandleScrollCollision(int collision_index) override;
		std::vector<Texture2D> GetTextures(std::unordered_map<int, Texture2D> texture_map) override;
};

struct CommandParams {
	bool fireable = false;
	bool movable = false;
	bool capturable = false;

	std::string text_content;
};

class OptionScrollMenu: public ScrollMenu {
	public: 
		bool fireable;
		bool moveable;
		bool captureable;

		OptionScrollMenu(const Vector2 &mouse_position, CommandParams params);
		~OptionScrollMenu();

		UiSignal HandleScrollCollision(int collision_index) override;
		std::vector<Texture2D> GetTextures(std::unordered_map<int, Texture2D> texture_map) override;
};
//shows when engine state is in UNIT_INFO or HEX_INFO 
class InfoPanel {
	public:
		Rectangle render_rect;
		std::vector<Rectangle> elements;
		std::optional<Slot> transformation = std::nullopt;

		InfoPanel();
		void renderElements(const engine::Game &engine_instance);
};

enum class ElemTypes {
	kUnitScroll = 0,
	kOptionScroll,
	kTaskScroll,
	kFiringText,
	// kInfo,
};


// what if i want to cancel mid-animation I need a reference for which element has a trans
// Transformations Are 1-1.
// When an element is deleted the correlating transformation needs to be deleted.
class Transformation {
	public:
		Slot self_key;
		Rectangle target_pos;
		Rectangle *position;

		Transformation(Rectangle *current_pos, Rectangle desired_pos);
		Transformation(const Transformation& copy) : self_key(copy.self_key), target_pos(copy.target_pos), position(copy.position){};
};

class UiManager {
	public:
		std::vector<Element> ui_elements;
		Camera2D &camera;
		std::unique_ptr<ScrollMenu> scrl_menu; // only one scroll menu
		std::vector<Text> messages;
		SlotMap<Transformation> transformations;
		InfoPanel info;

		UiManager(Camera2D &camera);
		UiSignal CollisionCheck();

		//modifies/overwrites scroll menus (Possibly info panels in the future)
		//if the new concrete class is equal to the old one, no allocation is made
		void createUiElem(Vector2 position, ElemTypes type, CommandParams params);
		void hideElements();

		void animate();
		void transform();
		//renders options menu
		void renderUi(const engine::Game &engine_instance);
		void renderText();
};
}
#endif
