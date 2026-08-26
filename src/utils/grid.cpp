#include <cmath>

// #include <iostream>
#include "../../include/utils/grid.hpp"
#include <engine/entities.hpp>
#include <raylib.h>

HexSpace::HexSpace (){
	x_position = 0;
	y_position = 0;
	occupier_key = std::nullopt;
	structure_key = std::nullopt;
	env_defense = 0;
};

namespace grid {

std::unordered_map<int, Texture2D> texture_map = std::unordered_map<int, Texture2D>();

void initGrid(const int row, const int col, std::vector<std::vector<HexSpace>> &grid_space){
	// i is our row (y) index
	// j is our column (x) index
	
	//hexagon math
	// radius = ScreenWidth / 24.0;
	// inradius = (radius * std::sqrtf(3.0)) / 2.0;

	// decide whether positional values should be normalized or not
	float x = inradius;
	float y = radius * 2;
	

	for(int i = 0; i < row; ++i){

		if (i % 2 == 0) {
			x = inradius * 2;
		} else {
			x = inradius * 3;
		}

		for(int j = 0; j < col; j++){
			HexSpace *CurrentHex = &grid_space[i][j];
			//cannot traverse off of the map 
		

			//remember, our hexagons are represented in a matrix
			//the origin is the top left or first index
			//therefore, north (or up) is -1 and south (down) is + 1 
		
			//left

			if(j != 0){
				CurrentHex->neighbors[WEST] = &grid_space[i][j - 1];
				if (i != row - 1) CurrentHex->neighbors[SOUTH_WEST] = &grid_space[i + 1][j - 1];
 				if( i != 0) CurrentHex->neighbors[NORTH_WEST] = &grid_space [i - 1][j - 1];
			} 
			
			//right
			if(j != col - 1){
				CurrentHex->neighbors[EAST] = &grid_space[i][j + 1];
				if(i != row - 1)CurrentHex->neighbors[SOUTH_EAST] = &grid_space[i + 1][j + 1];
				if (i != 0) CurrentHex->neighbors[NORTH_EAST] = &grid_space[i - 1][j + 1];

			}

			CurrentHex->x_position = x;
			CurrentHex->y_position = y;

			CurrentHex->indices.x = j;
			CurrentHex->indices.y = i;

			float x_offset = (sqrt(3) / 2) * radius;
			float y_offset = radius / 2;

			CurrentHex->vertices[0] = { 
				.x = CurrentHex->x_position + x_offset, 
				.y = CurrentHex->y_position + -y_offset,
			};

			CurrentHex->vertices[1] = { 
				.x = CurrentHex->x_position,
				.y = CurrentHex->y_position -(radius),
			};

			CurrentHex->vertices[2] = { 
				.x = CurrentHex->x_position - x_offset,
				.y = CurrentHex->y_position + - (y_offset),
			};

			CurrentHex->vertices[3] = { 
				.x = CurrentHex->x_position + - x_offset,
				.y = CurrentHex->y_position + y_offset,
			};

			CurrentHex->vertices[4] = { 
				.x = CurrentHex->x_position,
				.y = CurrentHex->y_position + radius,
			};

			CurrentHex->vertices[5] = { 
				.x = CurrentHex->x_position + x_offset,
				.y = CurrentHex->y_position + y_offset,
			};
	
			x += inradius * 2.0;
		}
		y += (int) (radius * 3) / 2.0;
	}
}

//iterate over building values in slotmap
void renderBuildings(const SlotMap<Building> &buildings, const bool debug){
	const float draw_width = inradius;
	const float draw_height = radius;

	const Texture2D hq_texture = texture_map[kHQ];
	const Texture2D warehouse = texture_map[kWarehouse];
	
	Rectangle building_rectangle = {
		.x = 0,
		.y = 0,
		.width = draw_width, 
		.height = draw_height,
	};

	Rectangle source_rectangle = {
		.x = 0,
		.y = 0,
		.width = (float)hq_texture.width,
		.height = (float)hq_texture.height,
	};

	Texture2D current_texture;
	for(Building structure: buildings.values){
		building_rectangle.x = structure.hex->x_position - inradius / 2;
		building_rectangle.y = structure.hex->y_position - radius / 2;

		if(structure.type == FACTORY){
			current_texture = warehouse;
		} else {
			current_texture = hq_texture;
		}

		source_rectangle.width = (float)current_texture.width;
		source_rectangle.height = (float)current_texture.height;

		DrawTexturePro(current_texture, source_rectangle, building_rectangle, {.x = 0, .y = 0}, 0, RAYWHITE);

		if(debug){
		    DrawRectangleLines(
			    building_rectangle.x, 
			    building_rectangle.y, 
			    building_rectangle.width, 
			    building_rectangle.height, 
			    RED
			    );
		}

	}

}

//add visibility rules
//iterator over unit values in slotmap
void renderUnits(const SlotMap<Unit> &units, bool debug){
	const float draw_width = inradius * 2;
	const float draw_height = radius * 2;

	const Texture2D solider_texture = texture_map[kDarkSolider]; 

	Color color;

	Rectangle unit_rectangle = {
		.x = 0,
		.y = 0,
		.width = draw_width / 2, 
		.height = draw_height / 2,
	};

	Rectangle source_rectangle = {
		.x = 0,
		.y = 0,
		.width = (float)solider_texture.width,
		.height = (float)solider_texture.height,
	};

	for(int i = 0; i < units.values.size(); ++i){
		Unit CurrentUnit = units.values[i];

		if(CurrentUnit.atks_left == 0){
			color = GRAY;
		} else {
			color = RAYWHITE;
		}

		unit_rectangle.x = CurrentUnit.render_rect.x;
		unit_rectangle.y = CurrentUnit.render_rect.y;

		source_rectangle.width = float(texture_map[kDarkSolider].width);
		source_rectangle.height = float(texture_map[kDarkSolider].height);

		DrawTexturePro(solider_texture, source_rectangle, unit_rectangle, {.x = 0, .y = 0}, 0, color);

		if(debug){
		    DrawRectangleLines(
			    CurrentUnit.render_rect.x, 
			    CurrentUnit.render_rect.y, 
			    CurrentUnit.render_rect.width, 
			    CurrentUnit.render_rect.height, 
			    RED
			    );
		}
	}
};

void renderGrid(
		std::vector<std::vector<HexSpace>> grid_space, 
		const bool debug
		){

	const float draw_width = inradius * 2;
	const float draw_height = radius * 2;

	Rectangle hexagon_rectangle = {
		.x = 0,
		.y = 0,
		.width = draw_width, 
		.height = draw_height,
	};

	Rectangle border_rectangle = {
		.x = 0,
		.y = 0,
		.width = draw_width, 
		.height = float(draw_height / 2.8461538), // the qoutient of the hexagon textures height divided by the borders height
	};

	//TEMPORARY

	int y_offset = 0;
	Rectangle source_rectangle = {
		.x = 0,
		.y = 0,
		.width = 0.0,
		.height = 0.0,
	};


	Vector2 hex_pos = {
	    .x = 0,
	    .y = 0,
	};

	for(int i = 0; i < grid_space.size(); ++i){
		y_offset = 0 * i;

		for(int j = 0; j < grid_space[i].size(); ++j){
			//draw hex
			source_rectangle.width = float(texture_map[kGrassHex].width);
			source_rectangle.height = float(texture_map[kGrassHex].height);

			const HexSpace *CurrentHex = &grid_space[i][j];
			hexagon_rectangle.x = CurrentHex->x_position - inradius; 
			hexagon_rectangle.y = CurrentHex->y_position - radius - y_offset; 

			DrawTexturePro(texture_map[kGrassHex], source_rectangle , hexagon_rectangle, hex_pos, 0, RAYWHITE);

			//draw border
			if(j == 0 or j == grid_space[i].size() - 1 or i == grid_space.size() - 1){

				border_rectangle.x = hexagon_rectangle.x + inradius - (std::sqrt(3) / 2)  * radius;
				border_rectangle.y = hexagon_rectangle.y + radius + radius / 2;

				source_rectangle.width = float(texture_map[kGrassBorder].width);
				source_rectangle.height = float(texture_map[kGrassBorder].height);

				DrawTexturePro(texture_map[kGrassBorder], source_rectangle , border_rectangle, hex_pos, 0, RAYWHITE);

			}

			if (debug){ 
			    DrawRectangleLines(
					    hexagon_rectangle.x, 
					    hexagon_rectangle.y, 
					    hexagon_rectangle.width, 
					    hexagon_rectangle.height, 
					    RED);
			    DrawCircle(CurrentHex->x_position, CurrentHex->y_position, 3.0, RED);

			    for(int i = 0; i < 6; ++i){
				    DrawCircle(CurrentHex->vertices[i].x, CurrentHex->vertices[i].y, 3.0, RED);
			    }
			}
		}
	}
}

void initAssets(){

	const Texture2D hex_grass = LoadTexture("../assets/Hex_Grass_Single.png");
	const Texture2D grass_border = LoadTexture("../assets/Hex_Grass_Offset.png");
	const Texture2D dark_solider = LoadTexture("../assets/Dark_Solider.png");
	
	const Texture2D fire_button = LoadTexture("../assets/Fire_Usable.png");
	const Texture2D fire_unusable = LoadTexture("../assets/Fire_Unusable.png");

	const Texture2D move_button = LoadTexture("../assets/Move_Usable.png");
	const Texture2D move_unusable = LoadTexture("../assets/Move_Unusable.png");

	const Texture2D cap_button = LoadTexture("../assets/Capture_Usable.png");
	const Texture2D cap_unusable = LoadTexture("../assets/Capture_Unusable.png");

	const Texture2D end_button = LoadTexture("../assets/End_Clicked.png");
	const Texture2D info_rectangle = LoadTexture("../assets/Info_Rectangle.png");
	const Texture2D hq = LoadTexture("../assets/HQ.png");
	const Texture2D warehouse = LoadTexture("../assets/Warehouse.png");

	texture_map[grid::kGrassHex] = hex_grass;
	texture_map[grid::kGrassBorder] = grass_border;
	texture_map[grid::kDarkSolider] = dark_solider;

	texture_map[grid::kFireButton] = fire_button;
	texture_map[grid::kFireButtonUnusable] = fire_unusable;

	texture_map[grid::kMoveButton] = move_button;
	texture_map[grid::kCaptureButtonUnusable] = move_unusable;

	texture_map[grid::kCaptureButton] = cap_button;
	texture_map[grid::kCaptureButtonUnusable] = cap_unusable;

	texture_map[grid::kEndButton] = end_button;
	texture_map[grid::kInfoRect] = info_rectangle;
	texture_map[grid::kHQ] = hq;
	texture_map[grid::kWarehouse] = warehouse;
}
}
