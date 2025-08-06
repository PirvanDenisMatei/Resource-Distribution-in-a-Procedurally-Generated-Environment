#include <iostream>
#include <SFML/Graphics.hpp>
#include <Vector>
#include <random>
#include "Screen.h"
#define TILE_SIDE 30
#define TILE_HEIGHT 50


using namespace std;

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(1815, 925), "Trade Simulation");
    
    int x_slide = 0, y_slide = 0;
    //cout << "x_slide: " << x_slide << " | y_slide: " << y_slide << endl;

    MapScreen map_screen(&window);
    //map_screen.find_shortest_path(130, 780);
    //map_screen.find_dijkstra_path(227, 421);
    map_screen.draw_all_tiles();

    sf::Clock keyboard_clock;
    sf::Clock mouse_clock;
    sf::Clock income_clock;

    /*Tile* first_road_tile = nullptr;
    bool is_second = false;*/

    while (window.isOpen())
    {
        

        sf::Time elapsed_keyboard = keyboard_clock.getElapsedTime();
        if (elapsed_keyboard.asMilliseconds() > 40) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && map_screen.get_x_slide() > 0)
            {
                x_slide -= 180;
                map_screen.slide_map(-180, 0);
                //cout << "x_slide: " << x_slide << " | y_slide: " << y_slide << endl;
                map_screen.draw_all_tiles();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && map_screen.get_x_slide() < 2700)
            {
                x_slide += 180;
                map_screen.slide_map(180, 0);
                //cout << "x_slide: " << x_slide << " | y_slide: " << y_slide << endl;
                map_screen.draw_all_tiles();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && map_screen.get_y_slide() > 0)
            {
                y_slide -= 100;
                map_screen.slide_map(0, -100);
                //cout << "x_slide: " << x_slide << " | y_slide: " << y_slide << endl;
                map_screen.draw_all_tiles();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && map_screen.get_y_slide() < 1600)
            {
                y_slide += 100;
                map_screen.slide_map(0, 100);
                //cout << "x_slide: " << x_slide << " | y_slide: " << y_slide << endl;
                map_screen.draw_all_tiles();
            }
            keyboard_clock.restart();
        }

        sf::Time elapsed_mouse = mouse_clock.getElapsedTime();
        if (elapsed_mouse.asMilliseconds() > 500) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
                if (mouse_position.x < 1860 && mouse_position.y < 950 && mouse_position.x >= 0 && mouse_position.y >= 0) {
                    map_screen.get_clicked_tile(mouse_position.x, mouse_position.y);
                }
                //cout << endl << mouse_position.x << " " << mouse_position.y << endl;
                /*if (mouse_position.x < 1860 && mouse_position.y < 950 && mouse_position.x >= 0 && mouse_position.y >= 0) {
                    Tile* clicked_tile = map_screen.get_clicked_tile(mouse_position.x, mouse_position.y);
                    if (is_second == false) {
                        first_road_tile = clicked_tile;
                        is_second = true;
                    }
                    else {
                        map_screen.find_dijkstra_path(first_road_tile->get_id(), clicked_tile->get_id());
                        map_screen.draw_all_tiles();
                        is_second = false;
                    }
                }*/
                
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
                map_screen.modify_printing(0);
                map_screen.draw_all_tiles();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
                map_screen.modify_printing(1);
                map_screen.draw_all_tiles();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
                map_screen.distribute_resources();
                map_screen.draw_all_tiles();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
                map_screen.modify_printing(2);
                map_screen.draw_all_tiles();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                map_screen.modify_printing(3);
                map_screen.draw_all_tiles();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                map_screen.empty_city_income();
                map_screen.draw_all_tiles();
            }
            
            mouse_clock.restart();
        }        
    }
	map_screen.delete_all_tiles();
    return 0;
}