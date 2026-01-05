#include "Algorithims.hpp"
#include "GUI.hpp"
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
//main de var gidip al demek için extern kullanıyoruz
extern int visitedCount;
extern long long duration;
extern Font statusFont;
//Ariel font kullanabilmek için gerekli değişken
extern ImFont* globalFont;
const float DRAW_CELL_SIZE = 15.0f;
// Dijkstra öncelikli kuyruğu için karşılaştırma operatörü

struct CompareDist {
    bool operator()(const Node* a, const Node* b) const {
        //uzaklığı az olan öncelikli dediğimiz satır
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

    //pencere donmasını önlemek için event kontrolü yapıyoruz
    Event event;
    while (window.pollEvent(event)) {
        // Çarpıya basılırsa programı kapatıp kaçalım
        if (event.type == Event::Closed) {
            window.close();
            exit(0); // Veya return; ama algoritmadan çıkmak zor olabilir
        }
    }

    // ekranı temizliyoruz
    window.clear(Color::White);
    // haritayı çiziyoruz
    drawDebugGrid(window, map);

    float mapHeight = ROW * DRAW_CELL_SIZE;
    float panelHeight = 200.0f;

    // çalışırkenki butonların kaybolduğu kısım için başka bir dikdörtgen çiziyoruz
    RectangleShape panelCover(Vector2f(window.getSize().x, panelHeight));
    panelCover.setPosition(0, mapHeight);
    panelCover.setFillColor(Color(40, 40, 40));
    panelCover.setOutlineThickness(2);
    panelCover.setOutlineColor(Color::Black);

    window.draw(panelCover);
    Text infoText;
    infoText.setFont(statusFont); // Main'den gelen font
    infoText.setCharacterSize(24);
    infoText.setFillColor(Color::Yellow); // Altın sarısı olsun

    // ekrana yazdırmak için string oluşturuyoruz
    std::string durum = "Hesaplamalar Suruyor...\nGezilen Kare: " + std::to_string(visitedCount);
    infoText.setString(durum);

    // Yazıyı panelin ortasına hizalıyoruz
    FloatRect textRect = infoText.getLocalBounds();
    infoText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    infoText.setPosition(window.getSize().x / 2.0f, mapHeight + panelHeight / 2.0f);

    window.draw(infoText);
    // ekrana bastığımız kısım
    window.display();

    // bekleme koyarak animasyon etkisi yaratıyoruz
    sleep(milliseconds(10));
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
        //komşuları gezidiğimiz yer
        for (int i = 0; i < 4; i++) {
            int nx = current->x + dr[i]; int ny = current->y + dc[i];
            if (nx >= 0 && nx < ROW && ny >= 0 && ny < COLUMN) {
                Node* neighbour = &map[nx][ny];
                //dijkstra oluğu için daha kısa bir yol bulunduysa güncelleme yapılıyor
                if (!neighbour->isWall && current->dist + 1 < neighbour->dist) {
                    neighbour->dist = current->dist + 1;
                    neighbour->parent = current;
                    neighbour->isVisited = true;
                    visitedCount++;
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
                        neighbour->heur = calculateHeur(neighbour->x, neighbour->y, finish->x, finish->y);
                        neighbour->dist = newdist;
                        neighbour->isVisited = true;
                        visitedCount++;
                        // f costunu hesaplayıp push ediyoruz kuyruk öncelik sırasını f cost a göre yapıyor zaten push etmemiz yeterli
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
                    visitedCount++;
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
                    visitedCount++;
                    neighbour->parent = current;
                    neighbour->dist = current->dist + 1;
                    que.push(neighbour);
                }
            }
        }
    }
}