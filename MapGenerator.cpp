#include "Settings.hpp"
#include <iostream>
#include <vector>
#include "Node.hpp"
#include "MapGenerator.hpp"
#include <algorithm>
#include<climits>
#include <random>
using namespace std;

struct DSU {
    vector<int> parent;

    DSU(int n) {
        parent.resize(n);
        for (int i=0;i<n;i++) {
            parent[i]=i;        // başlangıç için her oda kendi için kök
        }
    }

    int find(int n) {
        if (parent[n]==n) {
            return n;
        }
        return parent[n]=find(parent[n]);  //hızlandırma amaçlı
    }

    void unite(int i,int j) {
        int parent_i= find(i);
        int parent_j=find(j);
        if (parent_i != parent_j) { //döngü olmaması için köklerinin farklı olması lazım
            parent[parent_i]=parent_j;
        }
    }

};

struct Wall {
    int x,y;
    int r1x,r1y;// duvarın solu ve üstü
    int r2x,r2y;// duvarın sağı ve altı
};
void initMap(Node map[ROW][COLUMN]) {
    int targetX=51,targetY=51;
    for (int i=0;i<ROW;i++) {
        for (int j=0;j<COLUMN;j++) {
        map[i][j].x=i;
        map[i][j].y=j;
        map[i][j].parent=nullptr;
        map[i][j].isFinish=map[i][j].isStart=map[i][j].isWall=map[i][j].isVisited=false;
        map[i][j].heur= calculateHeur(i,j,targetX,targetY);
        map[i][j].dist=INT_MAX;
        map[i][j].fcost=INT_MAX;
        map[i][j].isPath=false;
        }
    }
    //başlangıç ve bitişi sol üst ve sağ alt olarak seçtim. Değiştirmek isterseniz sölersiniz
    map[0][0].isStart=true;
    map[0][0].dist=0;
    map[0][0].fcost=map[0][0].heur;
    map[targetX-1][targetY-1].isFinish=true;
}
void printMap(Node map[ROW][COLUMN]) {
    for (int i=0;i<ROW;i++) {
        for (int j=0;j<COLUMN;j++) {
            if (map[i][j].isStart) {
                cout<< 'S';
            }

            else if (map[i][j].isFinish) {
                cout<< 'F';
            }
            else if (map[i][j].isWall){
                cout<<'#';
            }
            else {
                cout<< '.';
            }

        }
        cout<<"\n";
    }
}
void generate_w_kruskal(Node map[ROW][COLUMN]) {
    for (int i=0;i<ROW;i++) {
        for (int j=0;j<COLUMN;j++) {
            map[i][j].isWall=true;
        }
    }
    vector<Wall> walls;
    for (int i=1;i<ROW;i+=2) {
        for (int j=1;j<COLUMN;j+=2) {
            map[i][j].isWall=false;//tekli olanları duvar yapmıyoruz oda gibi
            if (j+2<COLUMN-1) { // odanın sağındaki duvar
                Wall w ={i,j+1,i,j,i,j+2};
                walls.push_back(w);
            }
            if (i+2 < ROW-1) {  // odanın solundaki duvar
                Wall w = {i+1, j, i, j, i+2, j};
                walls.push_back(w);
            }
        }
    }
    //rasgelelik için duvar listesini karıştırıyoruz
    random_device rd;
    mt19937 g(rd());
    shuffle(walls.begin(), walls.end(), g);

    DSU dsu(ROW*COLUMN);
    for (const auto &w:walls) {
        int id1=w.r1x*COLUMN+w.r1y;
        int id2=w.r2x*COLUMN+w.r2y;

        if (dsu.find(id1)!=dsu.find(id2)) {//bağlı değiller ise (döngü olmucaksa)
            dsu.unite(id1,id2);
            map[w.x][w.y].isWall = false; // duvarı kırıp yol yapıyoruz
        }
    }

    //giriş çıkış kapanmasın diye manuel açtım
    map[0][0].isWall=false;
    map[0][1].isWall=false;
    map[1][0].isWall=false;
    map[ROW-1][COLUMN-1].isWall=false;
    map[ROW-1][COLUMN-2].isWall=false;
    map[ROW-2][COLUMN-1].isWall=false;

}

