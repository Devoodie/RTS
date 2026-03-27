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

void playerInit(std::vector<Player> &player, int playerCount);

bool collisionCheck(std::vector<std::vector<HexSpace>> grid_space);

void versus(std::vector<Player> &player, int currentPlayer, int player_count, std::vector<std::vector<HexSpace>> grid_space);
}

//class Collider {};
//
#endif
