#ifndef NODE_HPP
#define NODE_HPP
#include <math.h>
using namespace std;
typedef struct Node {
    Node* parent;
    int x,y;
    int heur,dist,fcost;
    bool isVisited,isWall,isFinish,isStart,isPath;
}Node;
inline int calculateHeur(int x,int y,int targetX, int targetY ) {
    return abs(targetX-x)+abs(targetY-y);
}
#endif
