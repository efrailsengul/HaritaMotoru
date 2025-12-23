#include <iostream>
#include "Node.hpp"
#include <vector>
#include <stack>
#include <climits>
#include <queue>
#include "Algorithims.hpp"
#include "Settings.hpp"
using namespace std;

// önceliği düşük değere göre ayarlamak için yazılan fonksiyon
struct CompareDist {
    bool operator()(const Node* a, const Node* b) const {
        return a->dist > b->dist;
    }
};
struct CompareForAStar{
    bool operator()(Node* a, Node* b) {
        if (a->fcost==b->fcost) {
            return a->heur > b->heur;
        }
        return a->fcost > b->fcost;  //A* olduğu için önceliği fcost a göre yapıyoruz
    }
};

//komşuları gezme işleminin daha hızlı olması için
int dr[]={-1,1,0,0};
int dc[]={0,0,-1,1};
void backtrack(Node* finish) {
    Node* current = finish;
    int safetyCounter = 0;
    while (current != nullptr) {
        current->isPath = true;
        current = current->parent;
        safetyCounter++;
        if (safetyCounter > ROW * COLUMN) {
            cout << "HATA: Backtrack sonsuz donguye girdi veya start bulunamadi!" << endl;
            break;
        }
    }
}
void dijkstra(Node map[ROW][COLUMN], Node *start, Node *finish) {
    priority_queue<Node*, vector<Node*>, CompareDist> pq;
    start->isVisited=true;
    start->parent=nullptr;
    start->dist=0;
    pq.push(start);
    while (!pq.empty()) {
        Node* current=pq.top();
        pq.pop();
        if (current==finish) {
            backtrack(finish);
            return;
        }
        for (int i=0;i<4;i++) {
            int nx= current->x+dr[i];
            int ny= current->y+dc[i];
            if (nx>=0&&nx<ROW&& ny>=0&&ny<COLUMN) {
                Node* neighbour= &map[nx][ny];
                if (!neighbour->isWall) {
                    int newdist= current->dist+1; //ağırlıksız yaptığımız için her kare 1
                    if (newdist<neighbour->dist) {
                        neighbour->dist=newdist;
                        neighbour->parent=current;
                        neighbour->isVisited=true;
                        pq.push(neighbour);
                    }
                }
            }
        }
    }
}

void A_star(Node map[ROW][COLUMN], Node *start, Node *finish) {
    priority_queue<Node*,vector<Node*>,CompareForAStar> pq;
    start->dist=0;
    start->isVisited=true;
    start->parent=nullptr;
    start->fcost = start->dist + start->heur;

    pq.push(start);

    while (!pq.empty()) {
        Node* current = pq.top();
        pq.pop();

        if (current==finish) {
            backtrack(finish);
            return;
        }
        for (int i=0;i<4;i++) {
            int nx= current->x+dr[i];
            int ny= current->y+dc[i];
            if (nx>=0 && nx<ROW && ny>=0 && ny<COLUMN) {
                Node* neighbour = &map[nx][ny];

                if (!neighbour->isWall) {
                    int newdist = current->dist+1;  //ağırlıksız olduğu için her adım 1 maliyet

                    if (newdist<neighbour->dist) {
                        neighbour->dist=newdist;
                        neighbour->isVisited=true;
                        neighbour->fcost=neighbour->dist+neighbour->heur;
                        neighbour->parent=current;
                        pq.push(neighbour);
                    }
                }
            }
        }
    }
}
void DFS(Node* map[ROW][COLUMN], Node *start, Node *finish) {

}
void BFS(Node* map[ROW][COLUMN], Node *start, Node *finish) {

}





