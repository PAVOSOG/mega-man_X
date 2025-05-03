#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <cmath>
#include "player.hpp"

using namespace std;
using namespace sf;


void UpdateEnemies(float deltaTime, Screens screen);
void DrawEnemies(RenderWindow &window, Screens screen);
void StartEnemies(Screens screen, float deltaTime);

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

struct BoomBeen {
    Sprite shape;
    Texture gettingReady;
    Texture attack;
    int col;
    void start();
    void update();
};
struct BOSS
{
    Sprite shape;
    Texture flyTexture;
    Texture attackTexture;
    Texture deadTexture;
    void start();
    void update();
    void fly(float deltaTime);
    void attack();
    void dead();
    void flyMovement();
};

extern BDV ball_de_voux;
extern Jamminger jamminger[3];
extern BoomBeen boomBeen;

