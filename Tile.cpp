#include <iostream>
#include <random>
#include <unordered_map>
#include "Tile.h"

coordinates::coordinates() {};
coordinates::coordinates(int x, int y) { this->x = x; this->y = y; }
string coordinates::to_string() { return std::to_string(this->x) + " " + std::to_string(this->y); }
void coordinates::print_coordinates() { cout << x << ", " << y << endl; }
bool coordinates::operator==(coordinates c) {if (this->x == c.x && this->y == c.y) { return true; } else { return false; }}

string TerrainType_to_string(TerrainType type) {
	switch (type) {
	case SEA: { return "Sea"; }
	case OCEAN: { return "Ocean"; }
	case PLAINS: { return "Plains"; }
	case HILLS: { return "Hills"; }
	case MOUNTAIN: { return "Mountain"; }
	case SWAMP: { return "Swamp"; }
	case DESERT: { return "Desert"; }
	case DUNES: { return "Dunes"; }
	default: { return "unknown"; }
	}
}

TerrainType int_to_TerrainType(int type) {
	switch (type) {
	case 0: { return SEA; }
	case 1: { return OCEAN; }
	case 2: { return PLAINS; }
	case 3: { return HILLS; }
	case 4: { return MOUNTAIN; }
	case 5: { return SWAMP; }
	case 6: { return DESERT; }
	case 7: { return DUNES; }
	}
}

int TerrainType_cost(TerrainType terrain) {
	switch (terrain) {
	case 0: { return 10; } //sea
	case 1: { return 30; } //ocean
	case 2: { return 2; } //plains
	case 3: { return 6; } //hills
	case 4: { return 20; } //mountains
	case 5: { return 8; } //swamp
	case 6: { return 6; } //desert
	case 7: { return 20; } //dunes
	}
}

Tile::Tile(int id, int x, int y, int px, int py, TerrainType terrain) {
	this->id = id;
	this->city_id = -1;

	this->normal_coords.x = x;
	this->normal_coords.y = y;

	this->projected_coords.x = px;
	this->projected_coords.y = py;

	this->cost = TerrainType_cost(terrain);
	this->terrain = terrain;

	switch (this->terrain) {
	case SEA: {
		int what_good = rand() % 101;
		if (what_good < 90) {
			this->value = 1;
			//this->good = "fish";
		}
		else {
			this->value = 3;
			//this->good = "pearls";
		}
		break;
	}
	case OCEAN: {
		int what_good = rand() % 101;
		if (what_good < 95) {
			this->value = 2;
			//this->good = "ocean fish";
			break;
		}
		this->value = 10;
		//this->good = "whales";
		break;
	}
	case PLAINS: {
		int what_good = rand() % 101;
		if (what_good < 60) {
			this->value = 1;
			//this->good = "wheat";
			break;
		}
		if (what_good < 80) {
			this->value = 2;
			//this->good = "fruit";
			break;
		}
		this->value = 5;
		//this->good = "textiles";
		break;
	}
	case HILLS: {
		int what_good = rand() % 101;
		if (what_good < 50) {
			this->value = 2;
			//this->good = "lumber";
			break;
		}
		if (what_good < 70) {
			this->value = 3;
			//this->good = "grapes";
			break;
		}
		if (what_good < 90) {
			this->value = 5;
			//this->good = "wool";
			break;
		}
		this->value = 8;
		//this->good = "amber";
		break;
	}
	case MOUNTAIN: {
		int what_good = rand() % 101;
		if (what_good < 80) {
			this->value = 2;
			//this->good = "stone";
			break;
		}
		if (what_good < 90) {
			this->value = 6;
			//this->good = "silver";
			break;
		}
		this->value = 12;
		//this->good = "gold";
		break;
	}
	}

	string texture_file, projected_texture_file;
	switch(terrain)
	{
	case 0: // sea
	{
		texture_file = "Tiles/Terrain_Sea/normal_tile_sea_v0.png"; //" + std::to_string(rand() % 4) + "
		projected_texture_file = "Tiles/Terrain_Sea/projected_tile_sea(1).png";
		break;
	}
	case 1: // ocean
	{
		texture_file = "Tiles/Terrain_Ocean/normal_tile_ocean_v0.png"; //" + std::to_string(rand() % 3) + "
		projected_texture_file = "Tiles/Terrain_Ocean/projected_tile_ocean(1).png";
		break;
	}
	case 2: // plains
	{
		texture_file = "Tiles/Terrain_Plains/normal_tile_plains_v0.png"; //" + std::to_string(rand() % 4) + "
		projected_texture_file = "Tiles/Terrain_Plains/projected_tile_plains(1).png";
		break;
	}
	case 3: // hills
	{
		texture_file = "Tiles/Terrain_Hills/normal_tile_hills.png";
		projected_texture_file = "Tiles/Terrain_Hills/projected_tile_hills(1).png";
		break;
	}
	case 4: // mountains
	{
		texture_file = "Tiles/Terrain_Mountains/normal_tile_mountain.png";
		projected_texture_file = "Tiles/Terrain_Mountains/projected_tile_mountain(1).png";
		break;
	}
	case 5: // swamp
	{
		texture_file = "Tiles/normal_tile_swamp.png";
		projected_texture_file = "Tiles/projected_tile_swamp.png";
		break;
	}
	case 6: // desert
	{
		texture_file = "Tiles/normal_tile_desert.png";
		projected_texture_file = "Tiles/projected_tile_desert.png";
		break;
	}
	case 7: // dunes
	{
		texture_file = "Tiles/normal_tile_dunes.png";
		projected_texture_file = "Tiles/projected_tile_dunes.png";
		break;
	}
	}

	if (!this->normal_texture.loadFromFile(texture_file)) { cout << "Error: couldn't load tile sprite" << endl; }
	this->normal_sprite.setTexture(this->normal_texture);
	this->normal_sprite.setPosition(sf::Vector2f(x, y));

	if (!this->projected_texture.loadFromFile(projected_texture_file)) { cout << "Error: couldn't load tile sprite" << endl; }
	this->projected_sprite.setTexture(this->projected_texture);
	this->projected_sprite.setPosition(sf::Vector2f(px, py));


}

int Tile::get_id() { return this->id; }
int Tile::get_cost() { return this->cost; }
int Tile::get_value() { return this->value; }
TerrainType Tile::get_terrain() { return this->terrain; }
coordinates Tile::get_normal_coordinates() { return this->normal_coords; }
coordinates Tile::get_center() { coordinates center(this->normal_coords.x + 29, this->normal_coords.y + 24); return center; }
coordinates Tile::get_projected_coordinates() { return this->projected_coords; }
sf::Texture Tile::get_normal_texture() { return this->normal_texture; }
sf::Texture Tile::get_projected_texture() { return this->projected_texture; }
sf::Sprite Tile::get_normal_sprite() { return this->normal_sprite; }
sf::Sprite Tile::get_projected_sprite() { return this->projected_sprite; }
int Tile::get_city_id() { return this->city_id; }

std::string get_foreign_city_name(const std::wstring& romanian_name) {
	static const std::unordered_map<std::wstring, std::string> reverse_map = {
		{L"Jupânești", "jakarta"},
		{L"Pietrești", "gujarat"},
		{L"Vulcana de Sus", "yerevan"},
		{L"Poiana", "aleppo"},
		{L"Răchițele", "riga"},
		{L"Popești", "lubeck"},
		{L"Scărișoara", "sevilla"},
		{L"Stupina", "venice"},
		{L"Fântâni", "amsterdam"},
		{L"Florești", "constantinople"},
		{L"Costești", "alexandria"},
		{L"Dumbrăveni", "zanzibar"},
		{L"Cireșeni", "cuzco"},
		{L"Codreni", "malacca"},
		{L"Popricani", "cahokia"},
	};

	auto it = reverse_map.find(romanian_name);
	if (it != reverse_map.end())
		return it->second;
	else
		return ""; // sau L"[necunoscut]"
}

void Tile::convert_to_city(wstring name) {
	string texture_file;
	switch (this->terrain) {
	case PLAINS: {
		texture_file = "Tiles/Terrain_Plains/city_tile_plains.png";
		break;
	}
	case HILLS: {
		texture_file = "Tiles/Terrain_Hills/city_tile_hills.png";
		break;
	}
	case MOUNTAIN: {
		texture_file = "Tiles/Terrain_Mountains/city_tile_mountain.png";
		break;
	}
	}

	if (!this->normal_texture.loadFromFile(texture_file)) { cout << "Error: couldn't load tile sprite" << endl; }
	this->normal_sprite.setTexture(this->normal_texture);
	this->normal_sprite.setPosition(sf::Vector2f(this->normal_coords.x, this->normal_coords.y));

	string file_name = get_foreign_city_name(name);

	if(!this->projected_texture.loadFromFile("Tiles/Counties/projected_city_" + file_name + ".png")) {cout << "Error: couldn't load tile sprite" << endl; }
	this->projected_sprite.setTexture(this->projected_texture);
	this->projected_sprite.setPosition(sf::Vector2f(this->projected_coords.x, this->projected_coords.y));

	this->terrain = CITY;
}

void Tile::modify_position(int x, int y) 
{ 
	this->normal_coords.x -= x;
	this->normal_coords.y -= y;
	this->normal_sprite.setPosition(sf::Vector2f(this->normal_coords.x, this->normal_coords.y));
}

void Tile::set_city(int city_id) {
	this->city_id = city_id;
}

string Tile::to_string() {
	string text_to_return;
	text_to_return += "Tile ID: " + std::to_string(this->id);
	text_to_return += "\nTile type: " + TerrainType_to_string(this->terrain);
	text_to_return += "\nTile cost: " + std::to_string(this->cost);
	text_to_return += "\nCoordinates: " + this->normal_coords.to_string();
	return text_to_return;
}