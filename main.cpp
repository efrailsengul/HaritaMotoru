#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-sfml.h>
#include <iostream>

#include "Algorithims.hpp"
#include "Node.hpp"
#include "MapGenerator.hpp"
#include "Settings.hpp"
using namespace sf;
const float CELL_SIZE = 15.0f;

// Renklendirme ve Çizim Fonksiyonu
void drawDebugGrid(RenderWindow& window, Node map[ROW][COLUMN]) {
    RectangleShape cell(Vector2f(CELL_SIZE - 1.0f, CELL_SIZE - 1.0f));

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++) {
            cell.setPosition(j * CELL_SIZE, i * CELL_SIZE);

            if (map[i][j].isStart) {
                cell.setFillColor(Color::Green);
            }
            else if (map[i][j].isFinish) {
                cell.setFillColor(Color::Red);
            }
            else if (map[i][j].isWall) {
                cell.setFillColor(Color::Black);
            }
            else if (map[i][j].isPath)
                cell.setFillColor(Color::Magenta);
            else if (map[i][j].isVisited) {
                cell.setFillColor(Color(100, 100, 255));
            }

            else {
                cell.setFillColor(Color(240, 240, 240));
            }

            window.draw(cell);
        }
    }
}
int main() {
    Node map[ROW][COLUMN];
    initMap(map);
    generate_w_kruskal(map);
     // dijkstra(map,&map[0][0],&map[ROW-1][COLUMN-1]);
    // A_star(map,&map[0][0],&map[ROW-1][COLUMN-1]);
    RenderWindow window(VideoMode(COLUMN * CELL_SIZE, ROW * CELL_SIZE), "Algoritma Test Penceresi");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        Event event; // sf::Event -> Event
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) // sf::Event::Closed -> Event::Closed
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                // Hangi tuş? -> SPACE
                if (event.key.code == sf::Keyboard::Space) {
                    std::cout << "Space algilandi, algoritma basliyor..." << std::endl; // Konsoldan teyit et

                    // Başlangıç ve bitiş noktalarını burada bulup gönder
                    // Örnek: Start ve End node'larını bulup fonksiyona ver
                    DFS(window, map, &map[0][0], &map[ROW-1][COLUMN-1]);
                }
            }
            if (Mouse::isButtonPressed(Mouse::Left)) {
                Vector2i mousePos = Mouse::getPosition(window);
                int j = mousePos.x / CELL_SIZE;
                int i = mousePos.y / CELL_SIZE;

                if (i >= 0 && i < ROW && j >= 0 && j < COLUMN) {
                    if (!map[i][j].isStart && !map[i][j].isFinish) {
                        map[i][j].isWall = true;
                    }
                }
            }
        }

        window.clear(Color::White); // Arkaplan beyaz
        drawDebugGrid(window, map);
        window.display();
    }

    return 0;
}