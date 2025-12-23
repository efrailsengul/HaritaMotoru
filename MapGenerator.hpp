
#ifndef MAPGENERATOR_HPP
#define MAPGENERATOR_HPP
#include"Node.hpp"
#include "Settings.hpp"

void initMap(Node map[ROW][COLUMN]);
void printMap(Node map[ROW][COLUMN]);
void generate_w_kruskal(Node map[ROW][COLUMN]);
#endif