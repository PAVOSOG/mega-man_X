#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <cmath>

using namespace std;
using namespace sf;


void UpdateEnemies(float deltaTime);
void DrawEnemies(RenderWindow &window);
void StartEnemies();

// Structures
struct BDV {
    Sprite shape;
    Texture walk;
    int col;
    void start();
    void update(float deltaTime);
};

struct Jamminger {
    Sprite shape;
    Texture attack;
    Vector2f velocity;
    int yCord;
    int speed;
    int col;
    void start();
    void movement();
    void update();
};

struct GunVolt {
    Sprite shape;
    Texture gettingReady;
    Texture attack;
    int col;
    void start();
    void update();
};

extern BDV ball_de_voux;
extern Jamminger jamminger;
extern GunVolt gunvolt;

