#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
//#include <algorithm>
#include <queue>
#include "Screen.h"
#define TILE_SIDE 30
#define TILE_HEIGHT 50

using namespace std;

int get_tile_neighbour(int tile_id, directions dir) {
	int neighbour = -1;

	if (tile_id % 2 == 0) {
		switch (dir) {
		case NW:
			if (tile_id < 100 || tile_id % 100 == 0)
				neighbour = -1;
			else
				neighbour = tile_id - 101;
			break;
		case N:
			if (tile_id < 100)
				neighbour = -1;
			else
				neighbour = tile_id - 100;
			break;
		case NE:
			if (tile_id < 100 || tile_id % 100 == 99)
				neighbour = -1;
			else
				neighbour = tile_id - 99;
			break;
		case SE:
			if (tile_id % 100 == 99)
				neighbour = -1;
			else
				neighbour = tile_id + 1;
			break;
		case S:
			if (tile_id >= 4900)
				neighbour = -1;
			else
				neighbour = tile_id + 100;
			break;
		case SW:
			if (tile_id % 100 == 0)
				neighbour = -1;
			else
				neighbour = tile_id - 1;
			break;
		default:
			break;
		}
	}
	else {
		switch (dir) {
		case NW:
			if (tile_id % 100 == 0)
				neighbour = -1;
			else
				neighbour = tile_id - 1;
			break;
		case N:
			if (tile_id < 100)
				neighbour = -1;
			else
				neighbour = tile_id - 100;
			break;
		case NE:
			if (tile_id % 100 == 99)
				neighbour = -1;
			else
				neighbour = tile_id + 1;
			break;
		case SE:
			if (tile_id % 100 == 99 || tile_id >= 4900)
				neighbour = -1;
			else
				neighbour = tile_id + 101;
			break;
		case S:
			if (tile_id >= 4900)
				neighbour = -1;
			else
				neighbour = tile_id + 100;
			break;
		case SW:
			if (tile_id >= 4900 || tile_id % 100 == 0)
				neighbour = -1;
			else
				neighbour = tile_id + 99;
			break;
		default:
			break;
		}
	}
	return neighbour;
}

bool are_neighbours(int tile1, int tile2) {
	directions dirs[6] = {NW, N, NE, SE, S, SW};
	for (int i = 0; i < 6; i++) {
		if (tile2 == get_tile_neighbour(tile1, dirs[i])) {
			return true;
		}
	}
	return false;
}

class PerlinNoise {
	int p[512], permutation[256] = { 151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 
	};
	int offset;
public:
	PerlinNoise() {
		for (int i = 0; i < 256; i++) {
			p[256 + i] = p[i] = permutation[i];
		}
		offset = rand() % 100;
	}

	double fade(double t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	double lerp(double t, double a, double b) {
		return a + t * (b - a);
	}

	static double grad(int hash, double x, double y) {
		int h = hash & 7;
		double u = (h < 4) ? x : y;
		double v = (h < 4) ? y : x;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}


	TerrainType perlin_noise_value(int tile_id) {
		int tile_x = tile_id % 100;
		int tile_y = tile_id / 100;

		double square_width = 10.0;
		double square_height = 10.0;

		double x = double(tile_x / square_width);
		double y = double(tile_y / square_height);

		int square_x = int(tile_x / square_width) & 255;
		int square_y = int(tile_y / square_height) & 255;

		x -= floor(x);
		y -= floor(y);
		double u = fade(x);
		double v = fade(y);

		int A = p[square_x + offset] + square_y;
		int B = p[square_x + 1 + offset] + square_y;
		int C = p[square_x + offset] + square_y + 1;
		int D = p[square_x + 1 + offset] + square_y + 1;

		double value = lerp(v,
			lerp(u, grad(p[A], x, y), grad(p[B], x - 1, y)),
			lerp(u, grad(p[A + 1], x, y - 1), grad(p[B + 1], x - 1, y - 1)));

		if (value < -0.4)
			return OCEAN;
		else if (value < -0.2)
			return SEA;
		else if (value < 0.1)
			return PLAINS;
		else if (value < 0.4)
			return HILLS;
		else
			return MOUNTAIN;
	}
};



MapScreen::MapScreen(sf::RenderWindow *window) {
	this->window = window;
	this->x_slide = 0;
	this->y_slide = 0;
	this->printing_voronoi = false;
	this->printing_projection = false;
	this->drawing_scoreboard = false;
	this->drawing_roads = false;
	PerlinNoise perlin;

	//generarea de teren
	int counter = 0;
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			TerrainType tile_type = perlin.perlin_noise_value(counter);

			int tile_x, tile_y;
			int projected_tile_x, projected_tile_y;

			if (j % 2 == 0) {
				tile_x = (j / 2) * TILE_SIDE * 3;
				tile_y = i * TILE_HEIGHT;

				projected_tile_x = j / 2 * 6 * 3 + 903;
				projected_tile_y = i * 10 + 415;
			}
			else {
				tile_x = (j / 2) * TILE_SIDE * 3 + TILE_SIDE + (TILE_SIDE / 2);
				tile_y = i * TILE_HEIGHT + TILE_HEIGHT / 2;

				projected_tile_x = (j / 2) * 6 * 3 + 6 + (6 / 2) + 903;
				projected_tile_y = i * 10 + 5 + 415;
			}

			this->tiles.push_back(new Tile(counter, tile_x, tile_y, projected_tile_x, projected_tile_y, tile_type));
			counter++;
		}
	}

	//std::cout << "Generare de orase\n";
	int city_count = 15; //vom avea 15 orase
	//generam cele 15 orase
	for(int i = 0; i < city_count; i++)
	{
		//cout << "oras " << i << endl;
		//tipul de teren pe care se afla orasul
		int city_terrain = rand() % 100;
		TerrainType terrain;
		if (city_terrain < 60) { terrain = PLAINS; }
		else if (city_terrain < 90) { terrain = HILLS; }
		else { terrain = MOUNTAIN; }

		//luam toate tile-urile care sunt de tipul acelui teren
		vector<Tile*> terrain_tiles;

		copy_if(this->tiles.begin(), this->tiles.end(), back_inserter(terrain_tiles), [terrain](Tile* x) { return x->get_terrain() == terrain; });
		
		int city_tile_id;
		bool pozitie_ok;
		int incercari = 0;
		do
		{
			pozitie_ok = true;

			//luam un tile aleator de acel tip
			city_tile_id = terrain_tiles[rand() % terrain_tiles.size()]->get_id();
			
			//verificam daca nu este prea apropiat de alt oras
			for (int j = 0; j < this->cities.get_city_count(); j++) {
				if(this->get_tile_distance(city_tile_id, this->cities[j]->get_id()) < 300)
				{
					pozitie_ok = false;
				}
			}

			if (city_tile_id < 100 || city_tile_id > 4899 || city_tile_id % 100 == 0 || city_tile_id % 100 == 99) {
				pozitie_ok = false;
			}
			
			incercari++;
		} while (pozitie_ok == false && incercari < 20);
		//cout << "Am ajuns aici\n";
		if (pozitie_ok == true) {
			
			this->cities+=(this->tiles[city_tile_id]);

		}
	}

	std::cout << "Cities:\n";
	for (int i = 0; i < city_count; i++) {
		std::cout << "City " << i << ": " << cities[i]->get_id() << endl;
		this->tiles[cities[i]->get_id()]->convert_to_city(this->cities[i]->get_name());
	}

	//aici cream diagrama voronoi pt toate orasele generate
	this->cities.get_voronoi_diagram();

	//gasim drumurile dintre orase
	for (int city1 = 0; city1 < city_count; city1++) {
		for (int city2 = 0; city2 < city1; city2++) {
			if(this->get_tile_distance(this->cities[city1]->get_id(), this->cities[city2]->get_id()) < 1000)
			{
				this->cities.add_road(city1, city2, this->find_dijkstra_path(this->cities[city1]->get_id(), this->cities[city2]->get_id()));
			}
		}
	}

	//aici asignam fiecarui tile un oras in functie de distanta influentata de teren
	for (int id = 0; id < this->tiles.size(); id++) {
		if (this->tiles[id]->get_terrain() != CITY)
		{
			int city_candidate = this->cities[0]->get_id();
			for (int city = 1; city < city_count; city++) {
				if (this->get_tile_distance(this->cities[city]->get_id(), id) < this->get_tile_distance(city_candidate, id)) {
					city_candidate = this->cities[city]->get_id();
				}
			}
			this->tiles[id]->set_city(city_candidate);
		}
	}

	
}

void MapScreen::draw_all_tiles() {
	//stergem ce era inainte ca sa nu apara pe ecran
	this->window->clear(sf::Color::Black);

	sf::Font font;
	if (!font.loadFromFile("DejaVuSans.ttf")) {
		cout << "Couldn't find font file";
	}

	//afisam sprite-urile pt toate tile-urile
	int first_tile = (this->x_slide / 45) + (this->y_slide / 50 * 100);
	if (first_tile >= 100 && first_tile % 100 != 0) {
		first_tile = get_tile_neighbour(first_tile, NW);
	}
	else {
		if (first_tile >= 100 && first_tile % 100 == 0) {
			first_tile = get_tile_neighbour(first_tile, N);
		}
		
		if (first_tile < 100 && first_tile % 100 != 0) {
			first_tile -= 2;
		}
	}

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 44; j++) {
			int tile_id = first_tile + j + i * 100;
			if (tile_id < 5000)
			{
				this->window->draw(this->tiles[tile_id]->get_normal_sprite());
			}
		}
	}

	//afisam numele oraselor
	for (int city = 0; city < this->cities.get_city_count(); city++) {
		sf::Text text;
		text.setFont(font);
		text.setString(this->cities[city]->get_name());
		text.setCharacterSize(25);
		text.setFillColor(this->cities.get_city_by_id(this->cities[city]->get_id())->get_color());
		sf::FloatRect bounds = text.getLocalBounds();
		int text_x = this->tiles[this->cities[city]->get_id()]->get_center().x - (bounds.width / 2);
		int text_y = this->tiles[this->cities[city]->get_id()]->get_normal_coordinates().y - bounds.height - 10;
		text.setPosition(text_x, text_y);

		sf::RectangleShape text_box(sf::Vector2f(bounds.width + 15, bounds.height + 5));
		sf::Color black(0, 0, 0, 125);
		text_box.setPosition(text_x - 5, text_y + 5);
		text_box.setFillColor(black);
		sf::Color city_transparent_color = this->cities[city]->get_color();
		city_transparent_color.a = 125;
		text_box.setOutlineColor(this->cities[city]->get_color());
		text_box.setOutlineThickness(3);

		window->draw(text_box);
		window->draw(text);
	}

	//desenam drumurile
	if (this->drawing_roads == true) {
		this->draw_all_roads();
	}
	
	//desenam diagrama voronoi daca setarea este activa
	if (this->printing_voronoi == true) {
		this->draw_voronoi_zones();
	}

	//desenam proiectia hartii daca setarea este activa
	if (this->printing_projection == true) {
		this->draw_projected_map();
	}

	//desenam scoreboard-ul
	if (this->drawing_scoreboard == true) {
		this->draw_scoreboard();
	}
	
	this->window->display();
}

void MapScreen::draw_voronoi_zones() {
	//cout << "Desenam zonele diagramei voronoi\n";
	//vector<seg*>::iterator i;
	for (int i = 0; i < this->cities.output.size(); i++) {
		point p0 = this->cities.output[i]->start;
		point p1 = this->cities.output[i]->end;
		//cout << p0.x << " " << p0.y << " " << p1.x << " " << p1.y << endl;
		sf::Vertex linie[] =
		{
			sf::Vertex(sf::Vector2f(p0.x - this->x_slide, p0.y - this->y_slide), sf::Color::White),
			sf::Vertex(sf::Vector2f(p1.x - this->x_slide, p1.y - this->y_slide), sf::Color::White)
		};

		this->window->draw(linie, 2, sf::Lines);

		sf::Vertex linie2[] =
		{
			sf::Vertex(sf::Vector2f(p0.x - this->x_slide - 1, p0.y - this->y_slide), sf::Color::White),
			sf::Vertex(sf::Vector2f(p1.x - this->x_slide - 1, p1.y - this->y_slide), sf::Color::White)
		};

		this->window->draw(linie2, 2, sf::Lines);

		sf::Vertex linie3[] =
		{
			sf::Vertex(sf::Vector2f(p0.x - this->x_slide + 1, p0.y - this->y_slide), sf::Color::White),
			sf::Vertex(sf::Vector2f(p1.x - this->x_slide + 1, p1.y - this->y_slide), sf::Color::White)
		};

		this->window->draw(linie3, 2, sf::Lines);
	}
}

void MapScreen::draw_projected_map() {
	sf::RectangleShape minimap_box(sf::Vector2f(903, 505));
	minimap_box.setPosition(903,415);
	minimap_box.setFillColor(sf::Color::White);
	minimap_box.setOutlineColor(sf::Color::White);
	minimap_box.setOutlineThickness(2);

	sf::RectangleShape sight_plane(sf::Vector2f(361, 189));
	sight_plane.setPosition(903 + (this->x_slide * 0.2) , 415 + (this->y_slide * 0.2));
	sight_plane.setFillColor(sf::Color(0, 0, 0, 0));
	sight_plane.setOutlineColor(sf::Color::Red);
	sight_plane.setOutlineThickness(2);

	window->draw(minimap_box);
	for (int tile = 0; tile < this->tiles.size(); tile++) {
		this->window->draw(this->tiles[tile]->get_projected_sprite());
	}

	for (int road = 0; road < this->all_roads.size(); road++) {
		int road_beginning = this->all_roads[road][0];
		int road_ending = this->all_roads[road][this->all_roads[road].size() - 1];

		sf::Vertex muchie[] =
		{
			sf::Vertex(sf::Vector2f(this->tiles[road_beginning]->get_projected_coordinates().x + 6, this->tiles[road_beginning]->get_projected_coordinates().y + 5), sf::Color(124, 1, 1)),
			sf::Vertex(sf::Vector2f(this->tiles[road_ending]->get_projected_coordinates().x + 6, this->tiles[road_ending]->get_projected_coordinates().y + 5), sf::Color(124, 1, 1))
		};

		this->window->draw(muchie, 2, sf::Lines);
	}

	//window->draw(sight_plane);
}

void MapScreen::slide_map(int x, int y) {
	this->x_slide += x;
	this->y_slide += y;
	for (Tile* tile : this->tiles) {
		tile->modify_position(x, y);
	}
}

Tile* MapScreen::get_clicked_tile(int mouse_x, int mouse_y) {
	int lateral_offsets[15] = { 1, 3, 5, 6, 8, 9, 11, 13, 14, 16, 17, 19, 21, 22, 24 }, sum = 24;


	int tile_collumn = mouse_x / 90;
	int collumn_start = tile_collumn * 90;

	int tile_row = 0;
	int row_start = 0;

	if (mouse_x < collumn_start + 60) {
		//Nu este in una din locatiile secundare
		tile_row = mouse_y / 50;
		row_start = tile_row * 50;
		if (mouse_x - collumn_start < 15) {
			if (mouse_y - row_start < lateral_offsets[15 - (mouse_x - collumn_start)]) {
				//Este tile-ul din stanga sus;
				collumn_start -= 45;
				row_start -= 25;
			}
			else

				if (mouse_y - row_start >= 50 - lateral_offsets[15 - mouse_x - collumn_start]) {
					//Este tile-ul din stanga jos;
					collumn_start -= 45;
					row_start += 25;
				}
		}
		else {
			if (mouse_x - collumn_start >= 45) {
				if (mouse_y - row_start < lateral_offsets[mouse_x - collumn_start - 45]) {
					//Este tile-ul din dreapta sus";
					collumn_start += 45;
					row_start -= 25;
				}
				else
					if (mouse_y - row_start >= 50 - lateral_offsets[mouse_x - collumn_start - 45]) {
						//Este tile-ul din dreapta jos";
						collumn_start += 45;
						row_start -= 25;
					}
					else {
						//cout << "Este tile-ul principal din cadran\n";
					}
			}
			else {
				//cout << "Este tile-ul principal din cadran\n";
			}
		}
	}
	else {
		int tile_row = (mouse_y - 25) / 50;
		row_start = tile_row * 50 + 25;
		collumn_start += 45;
		//cout << "Suntem pe coloana secundara\n";
	}

	//cout << "row_start: " << row_start << " | collumn_start: " << collumn_start << endl;
	coordinates c;
	c.x = collumn_start;
	c.y = row_start;
	//acum collumn_start si row_start contin coordonatele x si y ale tile-ului;
	for (Tile* tile : this->tiles) {
		if (tile->get_normal_coordinates() == c) {
			std::cout << tile->to_string() << endl;
			if(tile->get_terrain() != CITY)
			{
				wcout << "Belongs to: " << this->cities.get_city_by_id(tile->get_city_id())->get_name() << endl << endl;
			} else {
				wcout << "City name: " << this->cities.get_city_by_id(tile->get_id())->get_name() << endl;
				for (int city = 0; city < 15; city++) {
					if (this->cities[city]->get_id() == tile->get_id()) {
						this->cities.get_city_grade(city);
					}
				}
				
				cout << endl;
			}
			return tile;
		}
	}
}

void MapScreen::draw_road(int tile1, int tile2) {
	directions dirs[6] = {NW, N, NE, SE, S, SW};
	directions dir = N;

	for (int i = 0; i < 6; i++) {
		if (tile2 == get_tile_neighbour(tile1, dirs[i])) {
			dir = dirs[i];
			break;
		}
	}

	sf::Texture road_texture;
	sf::Sprite road_sprite;
	int x, y;
	string texture_file;

	if (dir == N || dir == S) { texture_file = "Tiles/Roads/Road_N-S.png"; }
	if (dir == NW || dir == SE) { texture_file = "Tiles/Roads/Road_NW-SE.png"; }
	if (dir == SW || dir == NE) { texture_file = "Tiles/Roads/Road_SW-NE.png"; }

	if (dir == NE || dir == SE || dir == S) {
		if (dir != NE)
		{
			x = this->tiles[tile1]->get_normal_coordinates().x + 29;
			y = this->tiles[tile1]->get_normal_coordinates().y + 24;
		}
		else {
			x = this->tiles[tile1]->get_normal_coordinates().x + 29;
			y = this->tiles[tile1]->get_normal_coordinates().y - 3;
		}
	}
	else {
		if (dir != SW)
		{
			x = this->tiles[tile2]->get_normal_coordinates().x + 29;
			y = this->tiles[tile2]->get_normal_coordinates().y + 24;
		}
		else {
			x = this->tiles[tile2]->get_normal_coordinates().x + 29;
			y = this->tiles[tile2]->get_normal_coordinates().y - 2;
		}
	}

	if (!road_texture.loadFromFile(texture_file)) { std::cout << "Error: couldn't load tile sprite" << endl; }
	road_sprite.setTexture(road_texture);
	road_sprite.setPosition(sf::Vector2f(x, y));

	this->window->draw(road_sprite);
}

void MapScreen::draw_all_roads() {
	for (int i = 0; i < this->all_roads.size(); i++) {
		for (int j = 1; j < this->all_roads[i].size(); j++) {
			this->draw_road(this->all_roads[i][j - 1], this->all_roads[i][j]);
		}
	}
}

void MapScreen::draw_scoreboard() {
	int character_size = 20, character_space = 12;
	sf::Font font;
	if (!font.loadFromFile("DejaVuSans.ttf")) {
		cout << "Couldn't find font file";
	}

	vector<City*> ordered_cities;
	for (int city = 0; city < this->cities.get_city_count(); city++) {
		ordered_cities.push_back(this->cities[city]);
	}

	for (int city1 = 0; city1 < ordered_cities.size(); city1++) {
		for (int city2 = 0; city2 < city1; city2++) {
			if (ordered_cities[city1]->get_finances(0) > ordered_cities[city2]->get_finances(0)) {
				City* aux = ordered_cities[city1];
				ordered_cities[city1] = ordered_cities[city2];
				ordered_cities[city2] = aux;
			}
		}
	}

	int score_height = 0, score_width = 0;
	for (int city = ordered_cities.size() - 1; city >= 0; city--) {
		sf::Text text;
		text.setFont(font);
		text.setStyle(sf::Text::Bold);
		text.setString(to_wstring(city + 1) + L" " + ordered_cities[city]->get_name() + L": " + to_wstring(ordered_cities[city]->get_finances(0)));
		text.setCharacterSize(character_size);

		sf::FloatRect bounds = text.getLocalBounds();
		if (bounds.width > score_width) {
			score_width = bounds.width;
		}
		score_height += bounds.height;
		score_height += character_space;
	}

	sf::RectangleShape score_box(sf::Vector2f(score_width + 20, score_height + 10));
	sf::Color black(0, 0, 0, 165);
	score_box.setPosition(20, 900 - score_height);
	score_box.setFillColor(black);
	score_box.setOutlineColor(sf::Color::White);
	score_box.setOutlineThickness(2);
	window->draw(score_box);

	int text_print_height = 895;

	for (int city = ordered_cities.size() - 1; city >= 0; city--) {
		sf::Text text;
		text.setFont(font);
		text.setStyle(sf::Text::Bold);
		text.setString(to_wstring(city + 1) + L" " + ordered_cities[city]->get_name() + L": " + to_wstring(ordered_cities[city]->get_finances(0)));
		text.setCharacterSize(character_size);
		text.setFillColor(this->cities.get_city_by_id(ordered_cities[city]->get_id())->get_color());

		sf::FloatRect bounds = text.getLocalBounds();
		text_print_height -= bounds.height;
		text.setPosition(30, text_print_height);
		text_print_height -= character_space;

		window->draw(text);
	}
}

int MapScreen::get_tile_distance(int tile1, int tile2) {
	int distance_cost = sqrt(pow(this->tiles[tile1]->get_center().x - this->tiles[tile2]->get_center().x, 2) + pow(this->tiles[tile1]->get_center().y - this->tiles[tile2]->get_center().y, 2));
	return distance_cost;
}

struct search_segment {
	int tile_id;
	int distance;
	int predecessor;
	bool visited;

	search_segment(int tile_id, int distance) {
		this->tile_id = tile_id;
		this->distance = distance;
		this->predecessor = -1;
		this->visited = false;
	}
};

int get_id_index(vector<search_segment*> search, int tile_id) {
	for (int i = 0; i < search.size(); i++) {
		if (search[i]->tile_id == tile_id) {
			return i;
		}
	}
	return -1;
}

bool MapScreen::is_there_road(int tile1, int tile2) {
	for (int road = 0; road < this->all_roads.size(); road++) {
		int road_beginning = this->all_roads[road][0];
		int road_ending = this->all_roads[road][this->all_roads[road].size() - 1];
		if ((road_beginning == tile1 && road_ending == tile2) || (road_beginning == tile2 && road_ending == tile1)) {
			return 1;
		}
	}
	return 0;
}

int MapScreen::find_dijkstra_path(int tile1, int tile2) {
	for (int city = 0; city < 15; city++) {
		if (this->is_there_road(tile1, city) && this->is_there_road(city, tile2)) {
			cout << "Exista deja un drum prin alt oras\n";
			return -1;
		}
	}

	std::cout << "\nIncepe cautarea!!!" << endl;
	int inf = 10000;
	bool status = false;
	vector<search_segment*> search;
	
	search.push_back(new search_segment(tile1, 0));

	while (all_of(search.begin(), search.end(), [](search_segment* v) {return v->visited;}) == false) {
		int min_distance = inf;
		int u = -1;

		for (int i = 0; i < search.size(); i++) {
			if (search[i]->visited == false && search[i]->distance < min_distance) {
				min_distance = search[i]->distance;
				u = i;
			}
		}

		if (u == -1) {
			status = false;
			break;
		}

		if (search[u]->tile_id == tile2) {
			std::cout << "Am ajuns la final!!!" << endl;
			status = true;
			break;
		}

		search[u]->visited = true;

		//introduc toti vecinii nodului curent in vectorul de cautare daca nu erau deja
		directions dirs[6] = { NW, N, NE, SE, S, SW };
		for (int i = 0; i < 6; i++) {
			int neighbour = get_tile_neighbour(search[u]->tile_id, dirs[i]);
			if (neighbour != -1) {
				bool gasit = false;
				for (int j = 0; j < search.size(); j++) {
					if (neighbour == search[j]->tile_id) {
						gasit = true;
						break;
					}
					
				}
				if (gasit == false) {
					search.push_back(new search_segment(neighbour, inf));
				}
			}
		}

		for (int v = 0; v < search.size(); v++) {
			if (are_neighbours(search[u]->tile_id, search[v]->tile_id) == true && search[v]->visited == false) {
				int new_distance = search[u]->distance + ((this->tiles[search[u]->tile_id]->get_cost() + this->tiles[v]->get_cost()) / 2);
				if (new_distance < search[v]->distance) {
					search[v]->distance = new_distance;
					search[v]->predecessor = search[u]->tile_id;
				}
			}
		}
	}

	if(status == true)
	{
		vector<int> final_road;
		int current_tile = get_id_index(search, tile2);
		int road_cost = 0;

		while (search[current_tile]->tile_id != tile1) {
			final_road.insert(final_road.begin(), search[current_tile]->tile_id);
			current_tile = get_id_index(search, search[current_tile]->predecessor);
		}
		final_road.insert(final_road.begin(), tile1);

		for (int i = 1; i < final_road.size(); i++) {
			road_cost += (this->tiles[final_road[i - 1]]->get_cost() + this->tiles[final_road[i]]->get_cost()) / 2;
		}

		if(road_cost <= 150)
		{
			std::cout << "Drumul este bun!!!\n";
			this->all_roads.push_back(final_road);
			this->all_road_costs.push_back(road_cost);
		}
		else {
			std::cout << "Drumul este prea lung!!!\n";
		}

		std::cout << "Inceput: " << final_road[0] << " Final: " << final_road[final_road.size() - 1] << endl;
		std::cout << "Nr de tile-uri: " << final_road.size() << endl;
		for (int i = 0; i < final_road.size(); i++) {
			std::cout << final_road[i] << " ";
		}
		std::cout << "\nCost: " << road_cost << endl;
		return road_cost;
	}

	return 0;

	for (search_segment* seg : search) {
		delete seg;
	}
}

void MapScreen::distribute_resources() {
	//toate orasele colecteaza resursele care sunt in domeniul lor
	for (int tile = 0; tile < this->tiles.size(); tile++) {
		this->cities.get_city_by_id(this->tiles[tile]->get_city_id())->set_finances(1, this->tiles[tile]->get_value());
	}

	//acum se face schimbul
	//acesta este orasul curent, din care pleaca resurele
	for (int city1 = 0; city1 < this->cities.get_city_count(); city1++) {
		int initial_treasury = this->cities[city1]->get_finances(1);
		//cout << endl << this->cities[city1]->get_name() << endl;
		//cout << "Initial treasury: " << initial_treasury << endl;
		//calculam costul total al drumurilor care ies din acest oras
		
		//acesta este orasul in care se duc resursele
		for (int city2 = 0; city2 < this->cities.get_city_count(); city2++) {
			//daca exista drum intre ele se face un schimb
			if(this->cities.get_road_cost(city1, city2) > 0)
			{
				//procentajul de baza este egal intre toate drumurile
				double final_percentage = 0.6 / (this->cities.get_city_grade(city1));
				//cout << "\nProcentajul initial " << final_percentage << endl;
				//verificam care oras din relatie are mai multe drumuri. daca al doilea oras are mai multe drumuri, atunci vor merge mai multe resurse la el
				if (this->cities.get_city_grade(city1) > this->cities.get_city_grade(city2)) {
					//daca primul oras este mai mare, atunci din acesta pleaca cu 50% mai putine resure
					final_percentage = final_percentage * 0.5;
				}
				else {
					if (this->cities.get_city_grade(city1) < this->cities.get_city_grade(city2)) {
						//daca al doilea oras este mai mare, atunci din primul oras pleaca cu 50% mai multe resurse
						final_percentage = final_percentage * 1.5;
					}
				}
				//cout << "Procentajul dupa compararea drumurilor: " << final_percentage << endl;
				//in functie de costul drumului, pleaca cu intre 0% si 50% mai putine resurse
				double aux = this->cities.get_road_cost(city1, city2);
				final_percentage = final_percentage * (((- 1.0 / 3) * aux + 100) / 100);
				//cout << "Procentajul dupa influenta drumului cu cost " << this->cities.get_road_cost(city1, city2) << ": " << final_percentage << endl;

				//resursele pleaca din primul oras si ajung in al doilea, in functie de importanta si de cost
				double resources_transfered = this->cities[city1]->get_finances(1) * final_percentage;

				//cout << "Resursele transferate: " << resources_transfered << endl;
				this->cities[city2]->set_finances(1, resources_transfered);
				//cout << "Treasury din destinatie: " << this->cities[city2]->get_finances(1) << endl;
				this->cities[city1]->set_finances(1, -resources_transfered);
				//cout << "Treasury din sursa: " << this->cities[city1]->get_finances(1) << endl;
			}
		}

		//dupa ce s-au facut toate exporturile din acest oras, populatia va consuma 90% din resursele care au fost produse
		this->cities[city1]->set_finances(1, -0.9 * initial_treasury);
		//cout << "Resursele consumate: " << 0.9 * initial_treasury << endl;
		//cout << "Treasury din " << this->cities[city1]->get_name() << ": " << this->cities[city1]->get_finances(1) << endl << endl;
	}

	for (int city = 0; city < this->cities.get_city_count(); city++) {
		this->cities[city]->set_finances(0, this->cities[city]->get_finances(1));
		this->cities[city]->set_finances(1, -1 * this->cities[city]->get_finances(1));
	}
}

int MapScreen::get_x_slide() { return this->x_slide; }
int MapScreen::get_y_slide() { return this->y_slide; }

void MapScreen::modify_printing(int what) {
	switch (what) {
	case 0: { this->printing_voronoi = !this->printing_voronoi; break; }
	case 1: { this->printing_projection = !this->printing_projection; break; }
	case 2: { this->drawing_scoreboard = !this->drawing_scoreboard; break; }
	case 3: { this->drawing_roads = !drawing_roads; break; }
	}
}

void MapScreen::empty_city_income() {
	for (int city = 0; city < 15; city++) {
		this->cities[city]->set_finances(0, -1 * this->cities[city]->get_finances(0));
	}
}

void MapScreen::delete_all_tiles() {
	for (Tile* tile : this->tiles) {
		delete tile;
	}
	for (vector<int> road : this->all_roads) {
		road.clear();
	}

	for (vector<seg*>::iterator i = this->cities.output.begin(); i != this->cities.output.end(); ++i) {
		delete* i;
	}
	this->all_roads.clear();
	this->tiles.clear();
}

void MapScreen::operator+=(Tile* tile) {
	tiles.push_back(tile);
}