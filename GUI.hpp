#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "Node.hpp"
#include "Settings.hpp"

// --- GLOBAL FONT TANIMI ---
// Bu satır sayesinde tüm dosyalar bu fonta erişebilecek.
extern ImFont* globalFont;

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

// Harita Çizimi
inline void drawDebugGrid(sf::RenderWindow& window, Node map[ROW][COLUMN]) {
    const float DRAW_CELL_SIZE = 15.0f;
    sf::RectangleShape cell(sf::Vector2f(DRAW_CELL_SIZE - 1.0f, DRAW_CELL_SIZE - 1.0f));

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN; j++) {
            cell.setPosition(j * DRAW_CELL_SIZE, i * DRAW_CELL_SIZE);
            if (map[i][j].isStart) cell.setFillColor(sf::Color::Green);
            else if (map[i][j].isFinish) cell.setFillColor(sf::Color::Red);
            else if (map[i][j].isWall) cell.setFillColor(sf::Color::Black);
            else if (map[i][j].isPath) cell.setFillColor(sf::Color::Magenta);
            else if (map[i][j].isVisited) cell.setFillColor(sf::Color(100, 100, 255));
            else cell.setFillColor(sf::Color(240, 240, 240));
            window.draw(cell);
        }
    }
}

// ARTIK FONT PARAMETRESİ İSTEMİYORUZ (Otomatik globalFont kullanacak)
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