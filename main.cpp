#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-sfml.h>

int main() {
    // SFML 2.6 Pencere
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Harita Motoru (Stabil)");
    window.setFramerateLimit(60);

    // ImGui Başlat
    if (!ImGui::SFML::Init(window)) return -1;

    // NOT: Docking flag'ini sildik çünkü standart sürüm kullanıyoruz.
    // Artık pencereler sadece ana pencere içinde hareket eder.

    sf::Clock deltaClock;

    // Arka plan rengi
    float bgColor[3] = { 0.1f, 0.1f, 0.1f };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // --- Arayüz ---
        ImGui::Begin("Kontrol Paneli");
        ImGui::Text("Stabilite > Macera :)");
        ImGui::ColorEdit3("Arka Plan", bgColor);
        ImGui::End();
        // -------------

        window.clear(sf::Color(
            (int)(bgColor[0] * 255),
            (int)(bgColor[1] * 255),
            (int)(bgColor[2] * 255)
        ));

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}