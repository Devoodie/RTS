#include <iostream>
#include <raylib.h>
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

	//aidan optimize
	bool collisionCheck(std::vector<std::vector<HexSpace>> grid_space){
		Vector2 mouse_point = GetMousePosition();
		for(int i = 0; i < grid_space.size(); ++i){
			for(int j = 0; j < grid_space[i].size(); ++j){
				HexSpace &CurrentHex = grid_space[i][j];
				if(CheckCollisionPointPoly(mouse_point, CurrentHex.vertices, 6)) {
					std::cout << "Collision Hex " << j << ", " << i <<std::endl;
					if(CurrentHex.occupier != nullptr){
						//bring up unit stuff
					} 
					//do hexagon stuff
					return true;
				}
			}
		}
		return false;
	}

	void versus(std::vector<Player> &players, int playerIndex, int player_count, std::vector<std::vector<HexSpace>> grid_space){
		if(players.size() == 0) playerInit(players, player_count);

		Player current_player = players[playerIndex];

		//check collisions
		bool chng_state = collisionCheck(grid_space);

		//uichecks

		//check_hexagon
	}
}
