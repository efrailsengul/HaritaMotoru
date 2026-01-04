#include "Algorithims.hpp"
#include "GUI.hpp"      // Çizim fonksiyonu (drawUI) için
#include "Settings.hpp"
#include "Node.hpp"

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <climits>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-sfml.h>

using namespace sf;
using namespace std;

// Komşuları daha hızlı gezebilmek için yön dizileri
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

// Dijkstra öncelikli kuyruğu için karşılaştırma operatörü
struct CompareDist {
    bool operator()(const Node* a, const Node* b) const {
        return a->dist > b->dist;
    }
};
// A*  öncelikli kuyruğu için karşılaştırma operatörü
struct CompareForAStar {
    bool operator()(Node* a, Node* b) {

        return (a->fcost == b->fcost) ? (a->heur > b->heur) : (a->fcost > b->fcost);
    }
};

//Ekrana yazdırma fonksiyonu
void renderFrame(RenderWindow& window, Node map[ROW][COLUMN], Node* start, Node* finish) {
    static Clock renderClock;
    Time dt = renderClock.restart();

    // Delta time hatasını önlemek için güvenlik
    if (dt.asSeconds() <= 0.0f) dt = seconds(0.001f);

    ImGui::SFML::Update(window, dt);

    window.clear(Color::White);
    drawDebugGrid(window, map);

    int dummyAlgo = 0;
    // GUI'yi çiz ama butonları kilitle (true)
    // Font parametresini sildik çünkü GUI.hpp'de globalFont kullanıyoruz.
    drawUI(window, map, start, finish, dummyAlgo, true);

    ImGui::SFML::Render(window);
    window.display();
}

// Başlangıçtan bitişe yolu çizdiren fonksiyon
void backtrack(RenderWindow& window, Node* finish, Node map[ROW][COLUMN]) {
    stack<Node*> path;
    Node* current = finish;
    Node* startNode = nullptr;

    // Start node'u bul (UI düzgün çizilsin diye)
    for(int i=0; i<ROW; i++) for(int j=0; j<COLUMN; j++) if(map[i][j].isStart) startNode = &map[i][j];

    // Sondan başa doğru parentları stack e push ediyoruz
    while (current != nullptr) {
        path.push(current);
        current = current->parent;
    }

    // Stackten pop ederek başlangıçtan bitişe doğru yazdırıyoruz
    while (!path.empty()) {
        Node* temp = path.top(); path.pop();
        temp->isPath = true;
        // Her adımda ekrana yazdırıyoruz
        renderFrame(window, map, startNode, finish);

        sleep(milliseconds(20)); // Yolu çizerken biraz bekle

        Event event; while (window.pollEvent(event)) if (event.type == Event::Closed) { window.close(); return; }
    }
}

void dijkstra(RenderWindow& window, Node map[ROW][COLUMN], Node *start, Node *finish) {
    priority_queue<Node*, vector<Node*>, CompareDist> pq;
    start->isVisited = true;
    start->dist = 0;
    pq.push(start);

    while (!pq.empty()) {
        Node* current = pq.top(); pq.pop();
        if (current == finish) {
            backtrack(window, finish, map);
            return;
        }
        // Her adımda ekrana yazdırıyoruz
        renderFrame(window, map, start, finish);

        Event event; while (window.pollEvent(event)) if (event.type == Event::Closed) { window.close(); return; }

        for (int i = 0; i < 4; i++) {
            int nx = current->x + dr[i]; int ny = current->y + dc[i];
            if (nx >= 0 && nx < ROW && ny >= 0 && ny < COLUMN) {
                Node* neighbour = &map[nx][ny];
                if (!neighbour->isWall && current->dist + 1 < neighbour->dist) {
                    neighbour->dist = current->dist + 1;
                    neighbour->parent = current;
                    neighbour->isVisited = true;
                    pq.push(neighbour);
                }
            }
        }
    }
}

void A_star(RenderWindow& window, Node map[ROW][COLUMN], Node *start, Node *finish) {
    priority_queue<Node*, vector<Node*>, CompareForAStar> pq;
    start->dist = 0;
    start->isVisited = true;
    start->fcost = start->dist + start->heur;
    pq.push(start);

    while (!pq.empty()) {
        Node* current = pq.top(); pq.pop();
        // Bitişe geldiyse yolu yazdırıp fonksiyonu kapatıyoruz
        if (current == finish) {
            backtrack(window, finish, map);
            return;
        }
        // Her adımda ekrana basıyoruz
        renderFrame(window, map, start, finish);

        Event event; while (window.pollEvent(event)) if (event.type == Event::Closed) { window.close(); return; }

        for (int i = 0; i < 4; i++) {

            int nx = current->x + dr[i]; int ny = current->y + dc[i];

            if (nx >= 0 && nx < ROW && ny >= 0 && ny < COLUMN) {
                Node* neighbour = &map[nx][ny];

                if (!neighbour->isWall) {
                    int newdist = current->dist + 1;
                    if (newdist < neighbour->dist) {
                        neighbour->dist = newdist;
                        neighbour->isVisited = true;
                        neighbour->fcost = neighbour->dist + neighbour->heur;
                        neighbour->parent = current;
                        pq.push(neighbour);
                    }
                }
            }
        }
    }
}


void DFS(RenderWindow& window, Node map[ROW][COLUMN], Node *start, Node *finish) {
    // DFS olduğu için stack kullanıyoruz
    stack<Node*> st;
    start->isVisited = true;
    start->dist = 0;
    st.push(start);

    while (!st.empty()) {

        Node* current = st.top();
        st.pop();

        if (current == finish){
            backtrack(window, finish, map);
            return;
        }

        renderFrame(window, map, start, finish);

        Event event; while (window.pollEvent(event)) if (event.type == Event::Closed) { window.close(); return; }

        for (int i = 0; i < 4; i++) {
            int nx = current->x + dr[i]; int ny = current->y + dc[i];
            if (nx >= 0 && nx < ROW && ny >= 0 && ny < COLUMN) {
                Node* neighbour = &map[nx][ny];
                if (!neighbour->isWall && !neighbour->isVisited) {
                    neighbour->isVisited = true;
                    neighbour->parent = current;
                    neighbour->dist = current->dist + 1;
                    st.push(neighbour);
                }
            }
        }
    }
}

void BFS(RenderWindow& window, Node map[ROW][COLUMN], Node *start, Node *finish) {
    //BFS olduğu için kuyruk kullanıyoruz
    queue<Node*> que; start->isVisited = true; start->dist = 0; que.push(start);

    while (!que.empty()) {
        Node* current = que.front();
        que.pop();

        if (current == finish) {
            backtrack(window, finish, map);
            return;
        }

        renderFrame(window, map, start, finish);

        Event event; while (window.pollEvent(event)) if (event.type == Event::Closed) { window.close(); return; }

        for (int i = 0; i < 4; i++) {
            int nx = current->x + dr[i]; int ny = current->y + dc[i];
            if (nx >= 0 && nx < ROW && ny >= 0 && ny < COLUMN) {
                Node* neighbour = &map[nx][ny];
                if (!neighbour->isWall && !neighbour->isVisited) {
                    neighbour->isVisited = true;
                    neighbour->parent = current;
                    neighbour->dist = current->dist + 1;
                    que.push(neighbour);
                }
            }
        }
    }
}