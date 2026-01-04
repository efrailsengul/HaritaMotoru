#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "Node.hpp"
#include "Settings.hpp"


// --- GLOBAL FONT TANIMI ---
// Bu satır sayesinde tüm dosyalar bu fonta erişebilecek.
extern ImFont* globalFont;
extern sf::Texture tFloor;
extern sf::Texture tVisited;
extern sf::Texture tPath;
extern sf::Texture tWall;
extern sf::Texture tStart;
extern sf::Texture tFinish;

// Harita Sıfırlama
inline void resetMapForNewRun(Node map[ROW][COLUMN], Node* start) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++) {
            map[i][j].isVisited = false;
            map[i][j].isPath = false;
            map[i][j].parent = nullptr;
            map[i][j].dist = 999999;
            map[i][j].fcost = 999999;
            map[i][j].heur = 0;
        }
    }
    if(start) {
        start->dist = 0;
        start->isVisited = true;
    }
}

// Haritayı çizen ana fonksiyon
inline void drawDebugGrid(sf::RenderWindow& window, Node map[ROW][COLUMN]) {
    const float DRAW_CELL_SIZE = 15.0f;
    sf::Sprite sprite;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++) {
            float x = j * DRAW_CELL_SIZE;
            float y = i * DRAW_CELL_SIZE;

            // --- 2. HANGİ RESMİ SEÇECEĞİZ? (Önem Sırası) ---

            // Önce Duvar (Çalı)
            if (map[i][j].isWall) {
                sprite.setTexture(tWall);
            }
            // Sonra Başlangıç (Kazma)
            else if (map[i][j].isStart) {
                sprite.setTexture(tStart);
            }
            // Sonra Bitiş (Sandık)
            else if (map[i][j].isFinish) {
                sprite.setTexture(tFinish);
            }
            // --- Zemin Durumları ---

            // Yol (Altınlı Toprak) - En önemlisi bu!
            else if (map[i][j].isPath) {
                sprite.setTexture(tPath);
            }
            // Ziyaret Edilen (Kazılmış Toprak)
            else if (map[i][j].isVisited) {
                sprite.setTexture(tVisited);
            }
            // Hiçbiri değilse Normal Toprak
            else {
                sprite.setTexture(tFloor);
            }

            // --- 3. BOYUTLANDIRMA VE KONUM (Cilalama) ---

            // Resmi hücrenin içine tam sığdırmak için Scale (Ölçek) hesabı
            // Matematik: (Hedef Boyut) / (Resmin Gerçek Boyutu)
            if (sprite.getTexture()) { // Güvenlik kontrolü
                float scaleX = DRAW_CELL_SIZE / sprite.getTexture()->getSize().x;
                float scaleY = DRAW_CELL_SIZE / sprite.getTexture()->getSize().y;
                sprite.setScale(scaleX, scaleY);
            }

            sprite.setPosition(x, y);

            // 4. Çiz
            window.draw(sprite);
        }
    }
}

inline void drawUI(sf::RenderWindow& window, Node map[ROW][COLUMN], Node* start, Node* finish, int& selectedAlgo, bool disableButtons = false) {

    float mapBottomY = ROW * 15.0f;
    float windowWidth = (float)window.getSize().x;
    float panelHeight = 200.0f;

    ImGui::SetNextWindowPos(ImVec2(0, mapBottomY));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, panelHeight));

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.FrameRounding = 8.0f;
    style.ItemSpacing = ImVec2(8, 0);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("FullWidthPanel", nullptr, window_flags);

    // --- FONTU AKTİF ET (Global değişkeni kullan) ---
    if (globalFont) ImGui::PushFont(globalFont);

    // Dikey Ortalama
    float btnH = 60.0f;
    float startY = (panelHeight - btnH) * 0.5f;
    ImGui::SetCursorPosY(startY);

    // Yatay Hesap
    float contentW = ImGui::GetContentRegionAvail().x;
    float spacing = style.ItemSpacing.x;
    float btnW = (contentW - (3 * spacing)) * 0.25f;

    // 1. DIJKSTRA
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.13f, 0.6f, 0.37f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.7f, 0.45f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.3f, 1.0f));
    if (ImGui::Button("DIJKSTRA", ImVec2(btnW, btnH)) && !disableButtons) {
        if (start && finish) { resetMapForNewRun(map, start); selectedAlgo = 1; }
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    // 2. A*
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.3f, 0.76f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.4f, 0.85f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.2f, 0.6f, 1.0f));
    if (ImGui::Button("A*", ImVec2(btnW, btnH)) && !disableButtons) {
        if (start && finish) { resetMapForNewRun(map, start); selectedAlgo = 2; }
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    // 3. DFS
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.45f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.95f, 0.55f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.35f, 0.1f, 1.0f));
    if (ImGui::Button("DFS", ImVec2(btnW, btnH)) && !disableButtons) {
        if (start && finish) { resetMapForNewRun(map, start); selectedAlgo = 3; }
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    // 4. BFS
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.5f, 0.73f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.6f, 0.85f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.12f, 0.4f, 0.6f, 1.0f));
    if (ImGui::Button("BFS", ImVec2(btnW, btnH)) && !disableButtons) {
        if (start && finish) { resetMapForNewRun(map, start); selectedAlgo = 4; }
    }
    ImGui::PopStyleColor(3);

    // --- FONTU KAPAT ---
    if (globalFont) ImGui::PopFont();

    ImGui::End();
}