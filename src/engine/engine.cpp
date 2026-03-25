#include <iostream>
#include <engine/engine.hpp>
#include <engine/entities.hpp>

Player::Player(){
	units = std::vector<Unit>();
	units.reserve(12);
}

namespace engine {

	void playerInit(std::vector<Player> &player, int playerCount){
		if(player.size() == playerCount){
			std::cout << "PLAYER COUNT ALREADY MET!" << std::endl;
			return;
		}
		while(player.size() < playerCount) {
			player.emplace_back(Player());
		}
	}


	void versus(std::vector<Player> &players, int playerIndex, int player_count, std::vector<std::vector<HexSpace>> grid_space){
		if(players.size() == 0) playerInit(players, player_count);

		Player current_player = players[playerIndex];

		bool chng_state = collisionCheck(grid_space);

		//check collisions
		//check_hexagon
	}
}
