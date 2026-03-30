#ifndef RTS_ENGINE_H
#define RTS_ENGINE_H

#include "utils/grid.hpp"
#include <vector>

class Player {
	public:
		std::vector<Unit> units;
	//structures Aidan ARRAYLIST
	//resources

		Player();
};

namespace engine {

class Game{
	public:
		std::vector<Player> players;
		int player_index;
		int player_count;
		std::vector<std::vector<HexSpace>> grid_space;
		std::vector<Rectangle> ui_elements;

		void playerInit(int playerCount);
		bool uiCollisionCheck();
		bool collisionCheck();
		void versus();

		Game();
};

void handleCollision(HexSpace collided_hex);


}

//class Collider {};
//
#endif
