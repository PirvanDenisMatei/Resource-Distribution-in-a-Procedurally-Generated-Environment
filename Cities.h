#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>

using namespace std;

class City {

	int tile_id;
	wstring name;
	int income;
	int treasury;
	sf::Color color;
public:
	City(int tile_id, wstring name, int r, int g, int b);
	int get_id();
	wstring get_name();
	int get_finances(int which);
	//int get_income();
	//int get_treasury();
	void set_finances(int which, int amount);
	//void modify_income(int amount);
	//void modify_treasury(int amount);
	sf::Color get_color();
};


struct point {
	int x, y;
};

struct arc;
struct seg;

struct event {
	double x;
	point p;
	arc* a;
	bool valid;

	event(double x, point p, arc* a) {
		this->x = x;
		this->p = p;
		this->a = a;
		this->valid = true;
	}
};

struct arc {
	point p;
	arc* prev, * next;
	event* e;
	seg* s0, * s1;

	arc(point p, arc* a = 0, arc* b = 0) {
		this->p = p;
		this->prev = a;
		this->next = b;
		this->e = 0;
		this->s0 = 0;
		this->s1 = 0;
	}
};

struct seg {
	point start, end;
	bool done;

	seg(point p, vector<seg*>& output) {
		this->start = p;
		point end; end.x = 0; end.y = 0;
		this->end = end;
		this->done = false;

		output.push_back(this);
	}
	void finish(point p) {
		if (done) {
			return;
		}

		end = p;
		done = true;
	}
};



struct gt {
	bool operator()(point a, point b) 
	{ 
		return a.x == b.x ? a.y > b.y : a.x > b.x; 
	}
	bool operator()(event* a, event* b) 
	{ 
		return a->x > b->x; 
	}
};

class AllCities {
	vector<City*> all_cities;
	priority_queue<point,vector<point>,gt> points;
	priority_queue<event*,vector<event*>,gt> events;
	int X0, X1, Y0, Y1;
	arc* root = 0;
	int road_network[15][15];

	enum city_names { Jakarta, Gujarat, Yerevan, Aleppo, Riga, Lubeck, Sevilla, Venice, Amsterdam, Constantinople, Alexandria, Zanzibar, Cuzco, Malacca, Cahokia };
	vector<int> city_candidates;

public:
	vector<seg*> output;
	AllCities();
	int get_city_count();
	City* get_city_by_id(int id);
	int get_road_cost(int i, int j);
	int get_city_grade(int city);
	void add_road(int i, int j, int cost);
	void operator+=(Tile *city);
	City* operator[](int i);

	void get_voronoi_diagram();

	void process_point();
	void process_event();
	void front_insert(point p);

	bool circle(point a, point b, point c, double* x, point* o);
	void check_circle_event(arc* i, double x0);

	bool intersect(point p, arc* i, point* result = 0);
	point intersection(point p0, point p1, double l);

	void finish_edges();
	void print_output();
};