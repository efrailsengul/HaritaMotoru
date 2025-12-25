
#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#define ROW 51
#define COLUMN 51
#include "Node.hpp"
#include <SFML/Graphics.hpp>
inline void drawDebugGrid(sf::RenderWindow& window, Node map[ROW][COLUMN]);
#endif