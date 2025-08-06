#pragma once
#include <iostream>
#include <vector>
#include "Tile.h"
#include "Cities.h"

using namespace std;

enum directions {NW, N, NE, SE, S, SW};

int get_tile_neighbour(int tile_id, directions dir);

class MapScreen
{
	vector<Tile*> tiles;
	sf::RenderWindow* window;
	vector<vector<int>> all_roads;
	vector<int> all_road_costs;
	AllCities cities;
	bool printing_voronoi, printing_projection, drawing_scoreboard, drawing_roads;
	int x_slide;
	int y_slide;
public:
	MapScreen(sf::RenderWindow *window);
	void draw_all_tiles();
	void draw_projected_map();
	void slide_map(int x, int y);
	Tile* get_clicked_tile(int mouse_x, int mouse_y);
	void draw_road(int tile1, int tile2);
	void draw_all_roads();
	void draw_scoreboard();
	int get_tile_distance(int tile1, int tile2);
	int find_dijkstra_path(int tile1, int tile2);
	bool is_there_road(int tile1, int tile2);
	void draw_voronoi_zones();
	int get_x_slide();
	int get_y_slide();
	void distribute_resources();
	void modify_printing(int what);
	void empty_city_income();
	void delete_all_tiles();
	void operator+=(Tile* tile);
};