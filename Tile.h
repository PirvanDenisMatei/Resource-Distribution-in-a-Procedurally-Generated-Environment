#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

enum TerrainType {SEA, OCEAN, PLAINS, HILLS, MOUNTAIN, SWAMP, DESERT, DUNES , CITY};

string TerrainType_to_string(TerrainType type);
int TerrainType_cost(TerrainType terrain);

struct coordinates {
	int x, y;
	coordinates();
	coordinates(int x, int y);
	string to_string();
	void print_coordinates();
	bool operator==(coordinates c);          
};

                                             
class Tile {
protected:
	int id;
	int cost;
	int city_id;
	int value;
	//string good;
	TerrainType terrain;
	coordinates normal_coords;
	coordinates projected_coords;
	sf::Texture normal_texture;
	sf::Texture projected_texture;
	sf::Sprite normal_sprite;
	sf::Sprite projected_sprite;
public:
	Tile() {}
	Tile(int id, int x, int y, int px, int py, TerrainType terrain);
	int get_id();
	int get_cost();
	int get_value();
	TerrainType get_terrain();
	coordinates get_normal_coordinates();
	coordinates get_center();
	coordinates get_projected_coordinates();
	sf::Texture get_normal_texture();
	sf::Texture get_projected_texture();
	sf::Sprite get_normal_sprite();
	sf::Sprite get_projected_sprite();
	int get_city_id();
	void convert_to_city(wstring name);
	void modify_position(int x, int y);
	void set_city(int city_id);
	string to_string();
};