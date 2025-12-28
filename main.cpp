#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-sfml.h>
#include "Algorithims.hpp"
#include "Node.hpp"
#include "MapGenerator.hpp"
#include "Settings.hpp"
#include "GUI.hpp"
#include<iostream>
using namespace sf;

// --- GLOBAL DEĞİŞKEN TANIMI ---
ImFont* globalFont = nullptr;
const float DRAW_CELL_SIZE = 15.0f;
int main() {
    Node map[ROW][COLUMN];
    initMap(map);
    generate_w_perlin(map);

    int mapWidth = COLUMN * DRAW_CELL_SIZE;
    int mapHeight = ROW * DRAW_CELL_SIZE;
    int panelHeight = 200;

    RenderWindow window(VideoMode(mapWidth, mapHeight + panelHeight), "Algoritma Kontrol Merkezi", Style::Close);
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) return -1;
    Clock deltaClock;

    // --- FONTU YÜKLE VE GLOBAL DEĞİŞKENE ATA ---
    ImGuiIO& io = ImGui::GetIO();
    globalFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbi.ttf", 20.0f);

    if (globalFont == nullptr) {
        std::cout << "Font bulunamadi, varsayilan kullaniliyor." << std::endl;
    }
    (void)ImGui::SFML::UpdateFontTexture();
    // -------------------------------------------

    Node* startNode = &map[0][0];
    Node* finishNode = &map[ROW-1][COLUMN-1];
    int selectedAlgorithm = 0;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == Event::Closed) window.close();

            if (Mouse::isButtonPressed(Mouse::Left)) {
                if (!ImGui::GetIO().WantCaptureMouse) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    int j = mousePos.x / DRAW_CELL_SIZE;
                    int i = mousePos.y / DRAW_CELL_SIZE;
                    if (i >= 0 && i < ROW && j >= 0 && j < COLUMN)
                        if (!map[i][j].isStart && !map[i][j].isFinish) map[i][j].isWall = true;
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear(Color::White);
        drawDebugGrid(window, map);

        // PARAMETRE SİLİNDİ: Artık fontu veya algoritma fontunu parametre olarak vermiyoruz
        drawUI(window, map, startNode, finishNode, selectedAlgorithm, false);

        ImGui::SFML::Render(window);
        window.display();

        if (selectedAlgorithm != 0) {
            switch(selectedAlgorithm) {
                case 1: std::cout << "Baslatiliyor: Dijkstra\n"; dijkstra(window, map, startNode, finishNode); break;
                case 2: std::cout << "Baslatiliyor: A*\n"; A_star(window, map, startNode, finishNode); break;
                case 3: std::cout << "Baslatiliyor: DFS\n"; DFS(window, map, startNode, finishNode); break;
                case 4: std::cout << "Baslatiliyor: BFS\n"; BFS(window, map, startNode, finishNode); break;
            }
            selectedAlgorithm = 0;
        }
    }
    ImGui::SFML::Shutdown();
    return 0;
}