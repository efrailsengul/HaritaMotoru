#include "Settings.hpp"
#include <iostream>
#include <vector>
#include "Node.hpp"
#include "MapGenerator.hpp"
#include <algorithm>
#include<climits>
#include <random>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <cmath>
using namespace std;

// perlin noise algoritmasında bitişin etrafı kapanabiliyor bu durumun yaşanmaması için bir tane yol açıcı yolluyoruz
void ensurePath(Node map[ROW][COLUMN]) {
    // Başlangıç konumu
    int x = 1;
    int y = 1;

    // Hedef konumu
    int targetX = ROW - 2;
    int targetY = COLUMN - 2;

    // Hedefe varana kadar döngü
    while (x != targetX || y != targetY) {
        // Bulunduğumuz yeri duvar olmaktan çıkar (YOL YAP)
        map[x][y].isWall = false;

        // rasgelelik ekleyerek düz yol çizmesini engelliyoruz
        // %50 ihtimalle X ekseninde, %50 ihtimalle Y ekseninde ilerle
        int direction = rand() % 2;

        if (direction == 0) { // X Ekseninde Hareket
            if (x < targetX) x++;
            else if (x > targetX) x--;
        }
        else { // Y Ekseninde Hareket
            if (y < targetY) y++;
            else if (y > targetY) y--;
        }


        // Bazen yolu kalınlaştır (Sağını solunu da aç)
        if (rand() % 5 == 0) { // %20 ihtimalle etrafı da aç
            if (x + 1 < ROW) map[x+1][y].isWall = false;
            if (y + 1 < COLUMN) map[x][y+1].isWall = false;
        }
    }

    // Son olarak hedef noktayı da aç
    map[targetX][targetY].isWall = false;
}

// Basit Perlin Noise Sınıfı
class PerlinNoise {
    vector<int> p; // Rastgelelik permütasyon tablosu

public:
    // Kurucu Fonksiyon: Her seferinde farklı harita çıksın diye 'seed' alır
    PerlinNoise(unsigned int seed) {
        p.resize(256);
        iota(p.begin(), p.end(), 0); // 0'dan 255'e sayıları diz
        default_random_engine engine(seed);
        shuffle(p.begin(), p.end(), engine); // Sayıları karıştır
        p.insert(p.end(), p.begin(), p.end());    // Diziyi ikiye katla (taşmayı önlemek için)
    }

    // Koordinat ver, bana gürültü değerini (-1 ile 1 arası) dön
    double noise(double x, double y, double z) {
        int X = (int)floor(x) & 255;
        int Y = (int)floor(y) & 255;
        int Z = (int)floor(z) & 255;

        x -= floor(x); y -= floor(y); z -= floor(z);

        double u = fade(x), v = fade(y), w = fade(z);

        // Gradyan hesaplamaları (Haşmetli matematik kısmı)
        int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
        int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

        return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)),
                               lerp(u, grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z))),
                       lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), grad(p[BA + 1], x - 1, y, z - 1)),
                               lerp(u, grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1))));
    }

private:
    // Geçişleri yumuşatan fonksiyonlar
    double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    double lerp(double t, double a, double b) { return a + t * (b - a); }
    double grad(int hash, double x, double y, double z) {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
};

//kruskal algoritması için DSU
struct DSU {
    vector<int> parent;

    DSU(int n) {
        parent.resize(n);
        for (int i=0;i<n;i++) {
            parent[i]=i;        // başlangıç için her oda kendi için kök
        }
    }

    int find(int n) {
        if (parent[n]==n) {
            return n;
        }
        return parent[n]=find(parent[n]);  //hızlandırma amaçlı
    }

    void unite(int i,int j) {
        int parent_i= find(i);
        int parent_j=find(j);
        if (parent_i != parent_j) { //döngü olmaması için köklerinin farklı olması lazım
            parent[parent_i]=parent_j;
        }
    }

};

struct Wall {
    int x,y;
    int r1x,r1y;// duvarın solu ve üstü
    int r2x,r2y;// duvarın sağı ve altı
};
//haritayı başlatan fonksiyon
void initMap(Node map[ROW][COLUMN]) {
    int targetX=51,targetY=51;
    for (int i=0;i<ROW;i++) {
        for (int j=0;j<COLUMN;j++) {
        map[i][j].x=i;
        map[i][j].y=j;
        map[i][j].parent=nullptr;
        map[i][j].isFinish=map[i][j].isStart=map[i][j].isWall=map[i][j].isVisited=false;
        map[i][j].heur=0;
        map[i][j].dist=INT_MAX;
        map[i][j].fcost=INT_MAX;
        map[i][j].isPath=false;
        }
    }
    //başlangıç ve bitişi sol üst ve sağ alt olarak seçtik.
    map[0][0].isStart=true;
    map[0][0].dist=0;
    map[0][0].fcost=map[0][0].heur;
    map[targetX-1][targetY-1].isFinish=true;
}
void generate_w_kruskal(Node map[ROW][COLUMN]) {
    //her yeri önce duvar yapıyoruz
    for (int i=0;i<ROW;i++) {
        for (int j=0;j<COLUMN;j++) {
            map[i][j].isWall=true;
        }
    }
    vector<Wall> walls;
    for (int i=1;i<ROW;i+=2) {
        for (int j=1;j<COLUMN;j+=2) {
            map[i][j].isWall=false;//tekli olanları duvar yapmıyoruz oda gibi
            if (j+2<COLUMN-1) { // odanın sağındaki duvar
                Wall w ={i,j+1,i,j,i,j+2};
                walls.push_back(w);
            }
            if (i+2 < ROW-1) {  // odanın solundaki duvar
                Wall w = {i+1, j, i, j, i+2, j};
                walls.push_back(w);
            }
        }
    }
    //rasgelelik için duvar listesini karıştırıyoruz
    random_device rd;
    mt19937 g(rd());
    shuffle(walls.begin(), walls.end(), g);

    DSU dsu(ROW*COLUMN);
    for (const auto &w:walls) {
        int id1=w.r1x*COLUMN+w.r1y;
        int id2=w.r2x*COLUMN+w.r2y;

        if (dsu.find(id1)!=dsu.find(id2)) {//bağlı değiller ise (döngü olmucaksa)
            dsu.unite(id1,id2);
            map[w.x][w.y].isWall = false; // duvarı kırıp yol yapıyoruz
        }
    }

    //giriş çıkış kapanmasın diye manuel açtık
    map[0][0].isWall=false;
    map[0][1].isWall=false;
    map[1][0].isWall=false;
    map[ROW-1][COLUMN-1].isWall=false;
    map[ROW-1][COLUMN-2].isWall=false;
    map[ROW-2][COLUMN-1].isWall=false;

}
void generate_w_perlin(Node map[ROW][COLUMN]) {
    //rasgelelik için nesne
    random_device rd;
    // perlin noise matematik işlemleri için nesne
    PerlinNoise pn(rd());
    //ölçek ayarı mağaralar için büyüklük degeri gibi düsünülebilir
    float scale = 0.20f;

    //threshold fonksiyonun duvar yapıp yapmaması için gereken eşik değeri
    float threshold = 0.35f;
    for (int i=0;i<ROW;i++) {
        for (int j=0;j<COLUMN;j++) {
            if (map[i][j].isStart || map[i][j].isFinish) {
                continue;
            }
            double n = pn.noise(i*scale,j*scale,0.0);

            //değeri 0 ile 1 arasına çekmek için
            n = (n + 1.0) / 2.0;

            //eşik değeri istediğimiz aralıktaysa duvar yapıyoruz
            if (n > threshold-0.15 && n < threshold+0.15) {
                map[i][j].isWall=true;
            }
            else {
                map[i][j].isWall = false;
            }


        }

    }
    //giriş ve çıkışın etrafını sorun olmasın diye elle açıyoruz
    map[0][1].isWall = false;
    map[1][0].isWall = false;
    map[1][1].isWall = false;
    map[1][2].isWall = false;
    map[2][1].isWall = false;

    map[ROW-2][COLUMN-2].isWall = false;
    map[ROW-3][COLUMN-2].isWall = false;
    map[ROW-2][COLUMN-3].isWall = false;
    map[ROW-1][COLUMN-2].isWall=false;
    map[ROW-2][COLUMN-1].isWall=false;


    ensurePath(map);


}

