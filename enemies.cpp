#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "player.hpp"

using namespace std;
using namespace sf;
const float bdvframe = 0.1f;
float damageTimeframe = 2.0f;
float damageTimer = 0;

Screens oldScreen = Screens::MainMenu;

struct BDV
{
    Sprite shape;
    Texture walk;
    int col = 0;
    float bdvmovetimer;
    bool isDead;
    int health;
    int startX;
    int startY;
    int damage;
    void start(int x, int y)
    {
        col = 0;
        bdvmovetimer = 0;
        isDead = false;
        health = 4;
        startX = 0;
        startY = 0;
        damage = 1;

        startX = x;
        startY = y;
        walk.loadFromFile("enemies/Ball De Voux.png");
        shape.setScale(4, 4);
        shape.setPosition(x, y);
        shape.setTextureRect(IntRect(col * 50, 66, 50, 66));
    }
    void update(float deltaTime)
    {
        if (isDead)
        {
            return;
        }
        if (right)

            if (player.getPosition().x >= shape.getPosition().x - 1100)

            {
                shape.setTexture(walk);

                bdvmovetimer += deltaTime;
                if (bdvmovetimer >= bdvframe)
                {
                    bdvmovetimer = 0;
                    shape.setTextureRect(IntRect(col * 50, 66, 50, 66));

                    col = (col + 1) % 5;
                }
                if (shape.getPosition().x > 252)
                    shape.move(-2, 0);
            }
    }
};
struct Jamminger
{
    Sprite shape;
    Texture attack;
    Vector2f velocity;
    int damage;
    int yCord;
    int speed;
    int col;
    bool isDead;
    int health;
    int startX, startY;
    void start(int x, int y)
    {
        damage = 3;
        yCord;
        speed = 1;
        col = 0;
        isDead = false;
        health = 4;
        startX = 0, startY = 0;
        attack.loadFromFile("enemies/flying enemy.png");
        shape.setScale(4, 4);
        yCord = rand() % 400;
        startX = x;
        startY = y;
        shape.setPosition(Vector2f(x, y));
        shape.setTextureRect(IntRect(col * 50, 0, 50, 50));
    }
    void movement()
    {
        velocity = (player.getPosition()) - Vector2f(150, 150) - shape.getPosition();
        float mag = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
        if (mag != 0)
        {
            velocity.x = (velocity.x / mag) * speed;
            velocity.y = (velocity.y / mag) * speed;
        }
        shape.move(velocity);
    }
    void update()
    {
        if (isDead)
        {
            return;
        }
        if (player.getPosition().x >= shape.getPosition().x - 1100)
        {
            shape.setTexture(attack);
            movement();
            shape.setTextureRect(IntRect(col * 50, 0, 50, 50));
            col = (col + 1) % 5;
        }
    }
};


struct GunVolt
{
    Sprite shape;
    Texture gettingReady;
    Texture attack;
    int damage;
    int col;
    bool isDead;
    int health;
    void start(int x, int y)
    {
        damage = 3;
        col = 0;
        isDead = false;
        health = 4;
        gettingReady.loadFromFile("enemies/gun volt1.png");
        attack.loadFromFile("enemies/gun volt2.png");
        shape.setScale(5, 5);
        shape.setPosition(Vector2f(x, y));
        shape.setTexture(gettingReady);
        shape.setTextureRect(IntRect(col * 50, 0, 50, 65));
        col = (col + 1) % 7;
        shape.setTexture(attack);
        shape.setTextureRect(IntRect(col * 50, 0, 50, 70));
        col = (col + 1) % 5;
    }
    void update()
    {
        if (isDead)
        {
            return;
        }
        /*shape.setTexture(gettingReady);
        shape.setTextureRect(IntRect(col * 50, 0, 50, 65));
        col = (col + 1) % 7;*/
    }
};

struct BOSS
{
    Sprite shape;
    Texture flyTexture;
    Texture attackTexture;
    Texture deadTexture;
    int col;
    int row;
    bool isFlying;
    bool isAttacking;
    bool isDead;
    int health;
    bool remove;
    int damage;

    void start(int x, int y)
    {
        col = 0;
        row = 0;
        isFlying = true;
        isAttacking = false;
        isDead = false;
        health = 4;
        remove = false;
        damage = 3;
        flyTexture.loadFromFile("enemies/boss flying.png");
        shape.setScale(3.5, 3.5);
        shape.setPosition(x, y);
        flyMovement();
        fly();
    }
    void update()
    {
        if (isFlying)
        {
            flyMovement();
            fly();
        }
        else if (isAttacking)
            attack();
        if (isDead)
            dead();
    }
    void fly()
    {
        shape.setTexture(flyTexture);
        shape.setTextureRect(IntRect(col * 148, 0, 148, 120));
        col = (col + 1) % 3;
        if (shape.getPosition().y <= 200)
        {
            isFlying = false;
            isAttacking = true;
        }
    }
    void attack()
    {
        col = 0;
        attackTexture.loadFromFile("enemies/boss attacking.png");
        shape.setTexture(attackTexture);
        shape.setTextureRect(IntRect(col * 148, row * 128, 148, 128));
        col = (col + 1) % 3;
        row = (row + 1) % 2;
    }
    void dead()
    {
        deadTexture.loadFromFile("enemies/boss dead.png");
        shape.setTexture(deadTexture);
        shape.setTextureRect(IntRect(0, 3 * 112, 148, 112));
        shape.move(0, 0.5);
        if (shape.getPosition().y >= 800)
        {
            remove = true;
        }
    }
    void flyMovement()
    {
        if (shape.getPosition().y < 315)
            shape.move(0, 3);
    }
};

/////////////////////////// Enemies //////////////////////////////////////////////////////////

Jamminger jamminger[7];
const int jammingerCountV1 = 2;
const int jammingerCountV2 = 7;
Vector2f jammingerPosV1[jammingerCountV1] = {{3945, 0}, {3900, 0}};
Vector2f jammingerPosV2[jammingerCountV2] = {{3945, 0}, {3900, 0}, {5500, 0}, {6700, 0}, {6980, 0}, {7530, 0}, {7995, 0}};

BDV ball_de_voux[5];
const int ball_de_vouxCountV1 = 2;
const int ball_de_vouxCountV2 = 5;
Vector2f ball_de_vouxPos[ball_de_vouxCountV1] = {{2400, 200}, {2100, 200}};
Vector2f ball_de_vouxPosV2[ball_de_vouxCountV2] = {{}, {}, {}, {}, {}};

GunVolt gunvolt[3];
const int gunvoltCountV1 = 1;
const int gunvoltCountV2 = 2;
Vector2f gunvoltPosV1[gunvoltCountV1] = {{8470, 10}};
Vector2f gunvoltPosV2[gunvoltCountV2] = {{}, {}};

BOSS boss;
Vector2f bossPos = {500, 200};

//////////////////////////////////////////////////////////////////////////////////////////////////

int ball_de_vouxCount;
int jammingerCount;
int gunvoltCount;

void StartEnemies(Screens screen)
{
    if (screen == Screens::GamePlay1)
    {
        ball_de_vouxCount = ball_de_vouxCountV1;
        jammingerCount = jammingerCountV1;
        gunvoltCount = gunvoltCountV1;
    }
    else if (screen == Screens::GamePlay2)
    {
        ball_de_vouxCount = ball_de_vouxCountV2;
        jammingerCount = jammingerCountV2;
        gunvoltCount = gunvoltCountV2;
    }
    for (int i = 0; i < ball_de_vouxCount; i++)
    {
        ball_de_voux[i].start(ball_de_vouxPos[i].x, ball_de_vouxPos[i].y);
    }
    for (int i = 0; i < jammingerCount; i++)
    {
        jamminger[i].start(jammingerPosV1[i].x, jammingerPosV1[i].y);
    }
    for (int i = 0; i < gunvoltCount; i++)
    {
        gunvolt[i].start(gunvoltPosV1[i].x, gunvoltPosV1[i].y);
    }
    if (screen == Screens::GamePlay2)
    {
        boss.start(500, 200);
    }
}
bool updateEnemiesColor(int bulletIdx, int &health, Vector2f position, bool &isDead, Sprite &shape)
{
    if (isDead)
    {
        return false;
    }
    bool out = false;
    Vector2f bulletPos = player.bullets[bulletIdx].shape.getPosition();
    Vector2f enemyPos = shape.getPosition();
    FloatRect enemyBounds = shape.getGlobalBounds();
    float offsetX = enemyBounds.width / 2.0f;
    float offsetY = enemyBounds.height / 2.0f;
    if (abs(bulletPos.x - enemyPos.x) < offsetX && abs(bulletPos.y - enemyPos.y) < offsetY)
    {
        player.bullets.erase(player.bullets.begin() + bulletIdx);
        out = true;
        health--;
        if (health <= 0)
        {
            isDead = true;
        }
    }

    if (health == 3)
    {
        shape.setColor(sf::Color::Yellow);
    }
    else if (health == 2)
    {
        shape.setColor(sf::Color(0xFF, 0xA5, 0x00));
    }
    else if (health == 1)
    {
        shape.setColor(sf::Color::Red);
    }
    else if (health <= 0)
    {
        isDead = true;
    }
    return out;
}
void checkPlayerHealth(int damage, Sprite shape, bool isdead)
{
    if (player.isImmune || isdead)
    {
        return;
    }
    // cout << player.health << endl;
    Vector2f enemyPos = shape.getPosition();
    FloatRect enemyBounds = shape.getGlobalBounds();
    float offsetX = enemyBounds.width / 2.0f;
    float offsetY = enemyBounds.height / 2.0f;
    if (abs(player.getPosition().x - enemyPos.x) >= offsetX || abs(player.getPosition().y - enemyPos.y) >= offsetY)
    {
        return;
    }
    player.health -= damage;
    if (player.health <= 0)
    {
        player.isDead = true;
    }
    player.isImmune = true;
    damageTimer = 0;
}

void UpdateEnemies(float deltaTime, Screens screen)
{
    if (oldScreen != screen)
    {
        oldScreen = screen;
        StartEnemies(screen);
    }

    damageTimer += deltaTime;
    if (damageTimeframe <= damageTimer)
    {
        player.isImmune = false;
    }

    for (int i = 0; i < ball_de_vouxCount; i++)
    {
        checkPlayerHealth(ball_de_voux[i].damage, ball_de_voux[i].shape, ball_de_voux[i].isDead);
        ball_de_voux[i].update(deltaTime);
    }
    for (int i = 0; i < jammingerCount; i++)
    {
        checkPlayerHealth(jamminger[i].damage, jamminger[i].shape, jamminger[i].isDead);
        jamminger[i].update();
    }
    for (int i = 0; i < gunvoltCount; i++)
    {
        checkPlayerHealth(gunvolt[i].damage, gunvolt[i].shape, gunvolt[i].isDead);
        gunvolt[i].update();
    }
    if (screen == Screens::GamePlay2)
    {
        checkPlayerHealth(boss.damage, boss.shape, boss.isDead);
        boss.update();
    }
    for (int j = 0; j < player.bullets.size(); j++)
    {
        bool shouldBreak = false;
        // BDV
        for (int i = 0; i < ball_de_vouxCount; i++)
        {
            shouldBreak = updateEnemiesColor(j, ball_de_voux[i].health, ball_de_voux[i].shape.getPosition(), ball_de_voux[i].isDead, ball_de_voux[i].shape);
            if (shouldBreak)
                break;
        }
        if (shouldBreak)
            continue;

        // Jamminger
        for (int i = 0; i < jammingerCount; i++)
        {
            shouldBreak = updateEnemiesColor(j, jamminger[i].health, jamminger[i].shape.getPosition(), jamminger[i].isDead, jamminger[i].shape);
            if (shouldBreak)
                break;
        }
        if (shouldBreak)
            continue;

        // GunVolt
        for (int i = 0; i < gunvoltCount; i++)
        {
            shouldBreak = updateEnemiesColor(j, gunvolt[i].health, gunvolt[i].shape.getPosition(), gunvolt[i].isDead, gunvolt[i].shape);
            if (shouldBreak)
                break;
        }
        if (shouldBreak)
            continue;

        // Boss
        if (screen == Screens::GamePlay2 && updateEnemiesColor(j, boss.health, boss.shape.getPosition(), boss.isDead, boss.shape))
            continue;
    }
}

void DrawEnemies(RenderWindow &window, Screens screen)
{
    for (int i = 0; i < ball_de_vouxCount; i++)
        if (!ball_de_voux[i].isDead)
            window.draw(ball_de_voux[i].shape);
    for (int i = 0; i < jammingerCount; i++)
        if (!jamminger[i].isDead)
            window.draw(jamminger[i].shape);
    for (int i = 0; i < gunvoltCount; i++)
        if (!gunvolt[i].isDead)
            window.draw(gunvolt[i].shape);
    if (player.isImmune && !player.isDead)
    {
        sf::CircleShape immunityCircle;
        immunityCircle.setRadius(75);
        immunityCircle.setFillColor(sf::Color::Transparent);
        immunityCircle.setOutlineColor(sf::Color::White);
        immunityCircle.setOutlineThickness(3);
        immunityCircle.setOrigin(immunityCircle.getRadius(), immunityCircle.getRadius());
        immunityCircle.setPosition(player.getPosition());

        window.draw(immunityCircle);
    }
    if (screen == Screens::GamePlay2 && !boss.remove)
        window.draw(boss.shape);
}
