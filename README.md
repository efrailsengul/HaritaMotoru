# 🗺️ Harita Motoru & Algoritma Görselleştirici (Pathfinding Visualizer)

> **C++, SFML ve ImGui kullanılarak geliştirilmiş; A*, Dijkstra, BFS ve DFS algoritmalarını karşılaştırmalı olarak analiz eden gerçek zamanlı bir harita motoru.**


## 📖 Proje Hakkında

Bu proje, bilgisayar mühendisliği öğrencileri için grafik teorisi ve arama algoritmalarının çalışma mantığını görselleştirmek amacıyla geliştirilmiştir. Kullanıcılar, dinamik olarak oluşturulan haritalar üzerinde (Perlin Noise veya Maze) farklı algoritmaları çalıştırabilir, performanslarını (süre ve gezilen düğüm sayısı) anlık olarak takip edebilir.

Standart ızgara boyamanın ötesine geçilerek, **Dinamik Doku Yönetimi (Texture Swapping)** kullanılmış ve görsel zenginlik **Pixel Art** tarzıyla desteklenmiştir.

## ✨ Temel Özellikler

### 🧠 Arama Algoritmaları
* **A* (A-Star):** Manhattan Distance sezgisel (heuristic) yöntemi ile optimize edilmiş en kısa yol bulma.
* **Dijkstra:** Ağırlıklı grafik mantığıyla çalışan, kesin çözüm sunan algoritma.
* **BFS (Breadth-First Search):** Dalga yayılımı mantığıyla çalışan genişlik öncelikli arama.
* **DFS (Depth-First Search):** Derinlik öncelikli arama (Karşılaştırma ve farkı göstermek amaçlı).

### 🌍 Prosedürel Harita Üretimi
* **Doğal Mağara (Perlin Noise):** Rastgele ama doğal görünümlü, organik engeller oluşturur.
* **Karışık Labirent (Kruskal Algoritması):** Minimum Spanning Tree (MST) mantığıyla döngüsüz, mükemmel labirentler üretir.

### 📊 Analiz ve Arayüz
* **Canlı İstatistikler:** Algoritma çalışırken SFML ile anlık, bitince ImGui ile detaylı raporlama (Gezilen Kare Sayısı & İşlem Süresi ms).
* **Gelişmiş GUI:** ImGui kütüphanesi ile kullanıcı dostu kontrol paneli.
* **Görselleştirme:** "Duvar", "Zemin", "Yol" ve "Ziyaret Edilen" alanlar için özel doku (texture) atamaları.

## 🛠️ Teknolojiler

* **Dil:** C++ (C++17 Standartları)
* **Grafik Motoru:** SFML (Simple and Fast Multimedia Library)
* **Arayüz (GUI):** Dear ImGui
* **IDE:** CLion 
* **Build System:** CMake

## 🚀 Kurulum ve Çalıştırma

Projeyi kendi bilgisayarınızda çalıştırmak için aşağıdaki adımları izleyin:

### Gereksinimler
* C++ Derleyicisi (MinGW veya MSVC)
* CMake
* SFML Kütüphanesi
* ImGUI Kütüphanesi

### Adım Adım Kurulum

1.  **Depoyu Klonlayın:**
    ```bash
    git clone [https://github.com/kullaniciadi/HaritaMotoru.git](https://github.com/kullaniciadi/HaritaMotoru.git)
    cd HaritaMotoru
    ```

2.  **Bağımlılıkları Ayarlayın:**
    * `CMakeLists.txt` dosyasındaki SFML yollarının kendi bilgisayarınızdaki yollarla eşleştiğinden emin olun.

3.  **Derleme (Build):**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

4.  **Çalıştırma (Önemli!):**
    * Derlenen `.exe` dosyasının çalışması için `resources` (görseller, fontlar) klasörünün ve SFML `.dll` dosyalarının `.exe` ile aynı dizinde olduğundan emin olun.


## 🤝 Katkıda Bulunanlar

* **Efrail Toprak Şengül** - *Geliştirici & Algoritma Tasarımı*
* **Deniz Demirtaş** - *Geliştirici & Arayüz Tasarımı*
* **Cem Tekeşin** - *Geliştirici & Algoritma tasarımı*
* **Ali Buğrahan İpek** - *Raporlama & Analiz*

## 📄 Lisans

Bu proje MIT Lisansı ile lisanslanmıştır. Detaylar için `LICENSE` dosyasına bakabilirsiniz.

---
*Bu proje geliştirilirken kod optimizasyonu ve debug süreçlerinde yapay zeka asistanı desteği alınmıştır.*
