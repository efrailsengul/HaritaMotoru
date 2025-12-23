
#ifndef ALGORITHIMS_HPP
#define ALGORITHIMS_HPP
#include "Node.hpp"
#include "Settings.hpp"
void dijkstra(Node map[ROW][COLUMN],Node* start,Node* finish);
void A_star(Node map[ROW][COLUMN],Node* start,Node* finish);
void BFS(Node map[ROW][COLUMN],Node* start,Node* finish);
void DFS(Node map[ROW][COLUMN],Node* start,Node* finish);
#endif