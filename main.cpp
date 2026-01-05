#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-sfml.h>
#include "Algorithims.hpp"
#include "Node.hpp"
#include "MapGenerator.hpp"
#include "Settings.hpp"
#include "GUI.hpp"
#include<iostream>
#include<chrono>
using namespace sf;

// Global değişkenleri tanımlıyoruz
int visitedCount = 0;     // ziyaret edilen kareleri tutabilmek için
long long duration = 0;  //  çözüm süresi
ImFont* globalFont = nullptr; //font yükleme için
Font statusFont;    // çalışırkenki font için
const float DRAW_CELL_SIZE = 15.0f; // her bir node büyüklüğü(ekrandaki)

//resimleri koyabilmek için texture nesnesi oluşturuyoruz
 Texture tFloor,tVisited,tPath,tWall,tStart,tFinish;
int appState = 0; // 0: Menü, 1: Oyun
void loadTextures() {
    //Çizdiğimiz resimleri yüklediğimiz yer
    if (!tFloor.loadFromFile("path.png"))     printf("HATA: path.png bulunamadi!\n");
    if (!tVisited.loadFromFile("visited_path.png")) printf("HATA: visited_path.png bulunamadi!\n");
    if (!tPath.loadFromFile("route.png"))       printf("HATA: route.png bulunamadi!\n");
    if (!tWall.loadFromFile("wall.png"))       printf("HATA: wall.png bulunamadi!\n");
    if (!tStart.loadFromFile("start.png"))     printf("HATA: start.png bulunamadi!\n");
    if (!tFinish.loadFromFile("finish.png"))   printf("HATA: finish.png bulunamadi!\n");
    //font yüklemesi
    if (!statusFont.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {

        std::cout << "Font yuklenemedi!" << std::endl;
    }
    // resimlerin daha güzel gözükmesi için ayar
    tFloor.setSmooth(false);
    tVisited.setSmooth(false);
    tPath.setSmooth(false);
    tWall.setSmooth(false);
    tStart.setSmooth(false);
    tFinish.setSmooth(false);
}
int main() {
    Node map[ROW][COLUMN];
    //ekran için yükseklik ve genişlik hesaplaması
    int mapWidth = COLUMN * DRAW_CELL_SIZE;
    int mapHeight = ROW * DRAW_CELL_SIZE;
    int panelHeight = 200;

    RenderWindow window(VideoMode(mapWidth, mapHeight + panelHeight), "Algoritma Kontrol Merkezi", Style::Close);
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) return -1;
    Clock deltaClock;

    // arial font yükleme işlemi
    ImGuiIO& io = ImGui::GetIO();
    globalFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbi.ttf", 20.0f);

    if (globalFont == nullptr) {
        std::cout << "Font bulunamadi, varsayilan kullaniliyor." << std::endl;
    }
    (void)ImGui::SFML::UpdateFontTexture();
    // -------------------------------------------
    //başlangıç ve bitiş noktaları sabit sol üst ve sağ alt
    Node* startNode = &map[0][0];
    Node* finishNode = &map[ROW-1][COLUMN-1];
    int selectedAlgorithm = 0;
    //resimleri yüklüyoruz
    loadTextures();


    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == Event::Closed) window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear(Color::White);

       // durum =0 giriş ekranı
if (appState == 0) {

    Sprite bgSprite(tWall); // arka plan için duvar resmi olan çalıyı kullanıyoruz

    // ekranı dolduracak kadar döngü kuruyoruz
    for (int y = 0; y < window.getSize().y; y += bgSprite.getTexture()->getSize().y) {
        for (int x = 0; x < window.getSize().x; x += bgSprite.getTexture()->getSize().x) {
            bgSprite.setPosition(x, y);
            window.draw(bgSprite);
        }
    }

    //göz almaması için siyab overlay atıyoruz
    RectangleShape darkOverlay(Vector2f(window.getSize().x, window.getSize().y));
    darkOverlay.setFillColor(Color(0, 0, 0, 125)); // 180 = %70 koyuluk
    window.draw(darkOverlay);

    //  Menü makyajı
    // açılan pencereyi ortalıyoruz
    ImVec2 centerPos(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f)); // Tam merkezden hizala
    ImGui::SetNextWindowSize(ImVec2(500, 450)); // Biraz daha büyük pencere

    // kenarları yuvarlıyoruz
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

    //pencereyi şeffaf yapıyoruz
    ImGui::SetNextWindowBgAlpha(0.8f);

    ImGui::Begin("Main Menu", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

    // başlığı ayarlıyoruz
    // başlığı büyük yazabilmek için scale kullanıyoruz
    ImGui::SetWindowFontScale(2.0f);

    // başlığı ortalamak için hesap
    const char* titleText = "HARITA MOTORU";
    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize(titleText).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::TextColored(ImVec4(1, 0.8f, 0, 1), titleText); // Altın Sarısı Başlık

    ImGui::SetWindowFontScale(1.3f); // font büyüklüğünü diğer elemanlar için küçültüyoruz

    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();

    // alt başlık ortalama
    const char* subTitle = "Harita Yontemini Seciniz:";
    textWidth = ImGui::CalcTextSize(subTitle).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text(subTitle);

    ImGui::Spacing();
    ImGui::Spacing();

    //  butonlar
    //buton büyüklüklerini ayarlıyoruz
    float buttonWidth = 300.0f;
    float buttonHeight = 125.0f;
    float centerButtonX = (windowWidth - buttonWidth) * 0.5f;
    ImGui::SetCursorPosX(centerButtonX);

    if (ImGui::Button("DOGAL MAGARA\n(Perlin Noise)", ImVec2(buttonWidth, buttonHeight))) {
        initMap(map);
        generate_w_perlin(map);
        appState = 1;
    }

    ImGui::Spacing();

    ImGui::SetCursorPosX(centerButtonX);
    if (ImGui::Button("KARISIK LABIRENT\n(Kruskal)", ImVec2(buttonWidth, buttonHeight))) {
        initMap(map);
        generate_w_kruskal(map);
        appState = 1;
    }

    // çıkış butonu
    ImGui::Spacing();
    ImGui::SetCursorPosX(centerButtonX);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.2f, 0.2f, 1.0f)); // Kırmızı buton
    if (ImGui::Button("CIKIS", ImVec2(buttonWidth, 100))) {
        window.close();
    }
    ImGui::PopStyleColor();

    ImGui::End();
    ImGui::PopStyleVar(2); // Stilleri temizle
}
        // labirentin geldiği ekranın ayarları
        else{

        window.clear(Color::White);
        drawDebugGrid(window, map);
        drawUI(window, map, startNode, finishNode, selectedAlgorithm, false);

        if (selectedAlgorithm != 0) {
            visitedCount=0;
            duration=0;
            auto startTime = std::chrono::high_resolution_clock::now();//başlangıç zamanını alıyoruz
            switch(selectedAlgorithm) {
                case 1: std::cout << "Baslatiliyor: Dijkstra\n"; dijkstra(window, map, startNode, finishNode); break;
                case 2: std::cout << "Baslatiliyor: A*\n"; A_star(window, map, startNode, finishNode); break;
                case 3: std::cout << "Baslatiliyor: DFS\n"; DFS(window, map, startNode, finishNode); break;
                case 4: std::cout << "Baslatiliyor: BFS\n"; BFS(window, map, startNode, finishNode); break;
            }

            auto endTime = std::chrono::high_resolution_clock::now(); //bitiş zamanını alıyoruz
            // süreyi bulmak için bitiş-başlangıç yapıp ms cinsinden alıyoruz
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
            selectedAlgorithm = 0;
        }
    }
        ImGui::SFML::Render(window);
        window.display();
  }
    ImGui::SFML::Shutdown();
    return 0;
}