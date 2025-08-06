#include <iostream>
#include "Screen.h"

City::City(int tile_id, wstring name, int r, int g, int b) {
	this->tile_id = tile_id;
	this->name = name;
	this->color = sf::Color(r, g, b);
	this->income = 0;
	this->treasury = 0;
}

int City::get_id() { return this->tile_id; }
wstring City::get_name() { return this->name; }
int City::get_finances(int which) {
	if (which == 0) { return this->income; }
	else { return this->treasury; }
}
void City::set_finances(int which, int amount) {
	if (which == 0) {
		this->income += amount;
	} else { 
		this->treasury += amount; 
	}
}
sf::Color City::get_color() { return this->color; }


AllCities::AllCities() {
	X0 = 0; X1 = 4515; Y0 = 0; Y1 = 2525;
	this->city_candidates.push_back(Jakarta);
	this->city_candidates.push_back(Gujarat);
	this->city_candidates.push_back(Yerevan);
	this->city_candidates.push_back(Aleppo);
	this->city_candidates.push_back(Riga);
	this->city_candidates.push_back(Lubeck);
	this->city_candidates.push_back(Sevilla);
	this->city_candidates.push_back(Venice);
	this->city_candidates.push_back(Amsterdam);
	this->city_candidates.push_back(Constantinople);
	this->city_candidates.push_back(Alexandria);
	this->city_candidates.push_back(Zanzibar);
	this->city_candidates.push_back(Cuzco);
	this->city_candidates.push_back(Malacca);
	this->city_candidates.push_back(Cahokia);

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			this->road_network[i][j] = 0;
		}
	}
}

int AllCities::get_city_count() { return this->all_cities.size(); }
City* AllCities::get_city_by_id(int id) {
	for (int city = 0; city < this->all_cities.size(); city++) {
		if (this->all_cities[city]->get_id() == id) {
			return this->all_cities[city];
		}
	}
	return this->all_cities[0];
}

int AllCities::get_road_cost(int i, int j) { return this->road_network[i][j]; }

int AllCities::get_city_grade(int city) {
	int grade = 0;
	int total_road_cost = 0;
	for (int j = 0; j < 15; j++) {
		if (this->road_network[city][j] != 0) {
			grade++;
			total_road_cost += this->road_network[city][j];
		}
	}
	//cout << "Costul total al celor" << grade << " drumuri din orasul " << this->all_cities[city]->get_name() + ":\n " << total_road_cost << " din " << grade * 150 << endl;
	return grade;
}

void AllCities::add_road(int i, int j, int cost) {
	this->road_network[i][j] = cost;
	this->road_network[j][i] = cost;
}

void AllCities::operator+=(Tile *city) {
	int city_name = rand() % this->city_candidates.size();
	wstring name;
	int r = 0, g = 0, b = 0;

	switch (this->city_candidates[city_name]) {
		case Jakarta: { name = L"Jupânești"; r = 84; g = 12; b = 93; break; }
		case Gujarat: { name = L"Pietrești"; r = 135; g = 104; b = 85; break; }
		case Yerevan: { name = L"Vulcana de Sus"; r = 204; g = 119; b = 31; break; }
		case Aleppo: { name = L"Poiana"; r = 202; g = 150; b = 172; break; }
		case Riga: { name = L"Răchițele"; r = 8; g = 82; b = 165; break; }
		case Lubeck: { name = L"Popești"; r = 190; g = 70; b = 70; break; }
		case Sevilla: { name = L"Scărișoara"; r = 57; g = 160; b = 101; break; }
		case Venice: { name = L"Stupina"; r = 54; g = 167; b = 156; break; }
		case Amsterdam: { name = L"Fântâni"; r = 188; g = 211; b = 38; break; }
		case Constantinople: { name = L"Florești"; r = 189; g = 51; b = 164; break; }
		case Alexandria: { name = L"Costești"; r = 200; g = 200; b = 102; break; }
		case Zanzibar: { name = L"Dumbrăveni"; r = 130; g = 17; b = 17; break; }
		case Cuzco: { name = L"Cireșeni"; r = 157; g = 180; b = 180; break; }
		case Malacca: { name = L"Codreni"; r = 106; g = 71; b = 215; break; }
		case Cahokia: { name = L"Popricani"; r = 129; g = 214; b = 238; break; }
	}

	this->city_candidates.erase(city_candidates.begin() + city_name);
	this->all_cities.push_back(new City(city->get_id(), name, r, g, b));
	point p;
	p.x = city->get_center().x;
	p.y = city->get_center().y;
	this->points.push(p);
	//cout << "\ncity" << p.x << " " << p.y;
}

City* AllCities::operator[](int i) {
	if(i < this->get_city_count())
	{
		return this->all_cities[i];
	}
	else {
		return 0;
	}
}

void AllCities::get_voronoi_diagram() {
	cout << "\nIncepe cautarea diagramei voronoi\n";
	point p;

	while (!points.empty()) {
		//cout << "h";
		if (!events.empty() && events.top()->x <= points.top().x) {
			process_event();
		}
		else {
			process_point();
		}
	}

	while (!events.empty()) {
		process_event();
	}

	finish_edges();
	print_output();
	cout << "\nS-a format diagrama voronoi\n";
}

void AllCities::process_point() {
	point p = this->points.top();
	this->points.pop();

	this->front_insert(p);
}

void AllCities::process_event()
{
	// Get the next event from the queue.
	event* e = events.top();
	events.pop();

	if (e->valid) {
		// Start a new edge.
		seg* s = new seg(e->p, this->output);

		// Remove the associated arc from the front.
		arc* a = e->a;
		if (a->prev) {
			a->prev->next = a->next;
			a->prev->s1 = s;
		}
		if (a->next) {
			a->next->prev = a->prev;
			a->next->s0 = s;
		}

		// Finish the edges before and after a.
		if (a->s0) a->s0->finish(e->p);
		if (a->s1) a->s1->finish(e->p);

		// Recheck circle events on either side of p:
		if (a->prev) check_circle_event(a->prev, e->x);
		if (a->next) check_circle_event(a->next, e->x);
	}
	delete e;
}

void AllCities::front_insert(point p) {
	if (!root) {
		root = new arc(p);
		return;
	}

	// Find the current arc(s) at height p.y (if there are any).
	for (arc* i = root; i; i = i->next) {
		point z, zz;
		if (intersect(p, i, &z)) {
			// New parabola intersects arc i.  If necessary, duplicate i.
			if (i->next && !intersect(p, i->next, &zz)) {
				i->next->prev = new arc(i->p, i, i->next);
				i->next = i->next->prev;
			}
			else i->next = new arc(i->p, i);
			i->next->s1 = i->s1;

			// Add p between i and i->next.
			i->next->prev = new arc(p, i, i->next);
			i->next = i->next->prev;

			i = i->next; // Now i points to the new arc.

			// Add new half-edges connected to i's endpoints.
			i->prev->s1 = i->s0 = new seg(z, this->output);
			i->next->s0 = i->s1 = new seg(z, this->output);

			// Check for new circle events around the new arc:
			check_circle_event(i, p.x);
			check_circle_event(i->prev, p.x);
			check_circle_event(i->next, p.x);

			return;
		}
	}

	// Special case: If p never intersects an arc, append it to the list.
	arc* i;
	for (i = root; i->next; i = i->next); // Find the last node.

	i->next = new arc(p, i);
	// Insert segment between p and i
	point start;
	start.x = X0;
	start.y = (i->next->p.y + i->p.y) / 2;
	i->s1 = i->next->s0 = new seg(start, this->output);
}

void AllCities::check_circle_event(arc* i, double x0)
{
	// Invalidate any old event.
	if (i->e && i->e->x != x0)
		i->e->valid = false;
	i->e = NULL;

	if (!i->prev || !i->next)
		return;

	double x;
	point o;

	if (circle(i->prev->p, i->p, i->next->p, &x, &o) && x > x0) {
		// Create new event.
		i->e = new event(x, o, i);
		events.push(i->e);
	}
}

bool AllCities::circle(point a, point b, point c, double* x, point* o)
{
	// Check that bc is a "right turn" from ab.
	double det = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	if (det > -1e-10) return false;

	// Algorithm from O'Rourke 2ed p. 189.
	double A = b.x - a.x, B = b.y - a.y,
		C = c.x - a.x, D = c.y - a.y,
		E = A * (a.x + b.x) + B * (a.y + b.y),
		F = C * (a.x + c.x) + D * (a.y + c.y),
		G = 2 * (A * (c.y - b.y) - B * (c.x - b.x));

	if (G == 0) return false;  // Points are co-linear.

	// Point o is the center of the circle.
	o->x = (D * E - B * F) / G;
	o->y = (A * F - C * E) / G;

	// o.x plus radius equals max x coordinate.
	*x = o->x + sqrt(pow(a.x - o->x, 2) + pow(a.y - o->y, 2));
	return true;
}

bool AllCities::intersect(point p, arc* i, point* res)
{
	if (i->p.x == p.x) return false;

	double a, b;
	if (i->prev) // Get the intersection of i->prev, i.
		a = intersection(i->prev->p, i->p, p.x).y;
	if (i->next) // Get the intersection of i->next, i.
		b = intersection(i->p, i->next->p, p.x).y;

	if ((!i->prev || a <= p.y) && (!i->next || p.y <= b)) {
		res->y = p.y;

		// Plug it back into the parabola equation.
		res->x = (i->p.x * i->p.x + (i->p.y - res->y) * (i->p.y - res->y) - p.x * p.x)
			/ (2 * i->p.x - 2 * p.x);

		return true;
	}
	return false;
}

point AllCities::intersection(point p0, point p1, double l)
{
	point res, p = p0;

	if (p0.x == p1.x)
		res.y = (p0.y + p1.y) / 2;
	else if (p1.x == l)
		res.y = p1.y;
	else if (p0.x == l) {
		res.y = p0.y;
		p = p1;
	}
	else {
		// Use the quadratic formula.
		double z0 = 2 * (p0.x - l);
		double z1 = 2 * (p1.x - l);

		double a = 1 / z0 - 1 / z1;
		double b = -2 * (p0.y / z0 - p1.y / z1);
		double c = (p0.y * p0.y + p0.x * p0.x - l * l) / z0
			- (p1.y * p1.y + p1.x * p1.x - l * l) / z1;

		res.y = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
	}
	// Plug back into one of the parabola equations.
	res.x = (p.x * p.x + (p.y - res.y) * (p.y - res.y) - l * l) / (2 * p.x - 2 * l);
	return res;
}

void AllCities::finish_edges()
{
	// Advance the sweep line so no parabolas can cross the bounding box.
	double l = X1 + (X1 - X0) + (Y1 - Y0);

	// Extend each remaining segment to the new parabola intersections.
	for (arc* i = root; i->next; i = i->next)
		if (i->s1)
		{
			point p_right = intersection(i->p, i->next->p, l * 2);
			point p_left = intersection(i->p, i->next->p, -l * 2);

			double dxr = abs(p_right.x - i->s1->start.x);
			double dxl = abs(p_left.x - i->s1->start.x);
			i->s1->finish(dxr > dxl ? p_right : p_left);
		}
}

void AllCities::print_output()
{
	// Bounding box coordinates.
	//cout << X0 << " " << X1 << " " << Y0 << " " << Y1 << endl;

	// Each output segment in four-column format.
	vector<seg*>::iterator i;
	int k = 0;
	for (i = output.begin(); i != output.end(); i++) {
		point p0 = (*i)->start;
		point p1 = (*i)->end;
		cout << "\npunctul" << k << ": " << p0.x << " " << p0.y << " " << p1.x << " " << p1.y; k++;
	}
}