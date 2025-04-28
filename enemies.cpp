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

struct BDV
{
    Sprite shape;
    Texture walk;
    int col = 0;
    float bdvmovetimer = 0;
    bool isDead = false;
    int health = 4;
    void start()
    {
        walk.loadFromFile("enemies/Ball De Voux.png");
        shape.setScale(4, 4);
        shape.setPosition(2570, 220);
    }
    void update(float deltaTime)
    {
        if (isDead)
        {
            return;
        }
        shape.setTexture(walk);
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
        bdvmovetimer += deltaTime;
        if (bdvmovetimer >= bdvframe)
        {
            bdvmovetimer = 0;
            shape.setTextureRect(IntRect(col * 50, 66, 50, 66));

            col = (col + 1) % 5;
        }
        if (ball_de_voux.shape.getPosition().x > 252)
            shape.move(-2, 0);

        if (player.getPosition().x == shape.getPosition().x)
            isDead = true;
        for (int i = 0; i < player.bullets.size(); i++)
        {
            Vector2f bulletPos = player.bullets[i].shape.getPosition();
            Vector2f enemyPos = shape.getPosition();
            float offsetX = 10.0f;
            float offsetY = 200.0f;
            if (abs(bulletPos.x - enemyPos.x) < offsetX && abs(bulletPos.y - enemyPos.y) < offsetY)
            {
                player.bullets.erase(player.bullets.begin() + i);
                health--;
                if (health <= 0)
                {
                    isDead = true;
                }
            }
        }
    }
} ball_de_voux,ball_de_voux1[2],ball_de_voux2[5];
struct Jamminger
{
    Sprite shape;
    Texture attack;
    Vector2f velocity;
    int yCord;
    int speed = 1;
    int col = 0;
    bool isDead = false;
    void start()
    {
        attack.loadFromFile("enemies/flying enemy.png");
        shape.setScale(4, 4);
        yCord = rand() % 400;
        shape.setPosition(Vector2f(1800, jamminger.yCord));
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
        shape.setTexture(attack);
        movement();
        shape.setTextureRect(IntRect(col * 50, 0, 50, 50));
        col = (col + 1) % 5;
    }
} jamminger;
struct GunVolt
{
    Sprite shape;
    Texture gettingReady;
    Texture attack;
    bool isDead = false;
    int col = 0;
    void start()
    {
        gettingReady.loadFromFile("enemies/gun volt1.png");
        attack.loadFromFile("enemies/gun volt2.png");
        shape.setScale(5, 5);
        shape.setPosition(Vector2f(1000, 200));
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
} gunvolt;

struct BOSS
{
    Sprite shape;
    Texture flyTexture;
    Texture attackTexture;
    Texture deadTexture;
    int col = 0;
    int row = 0;
    bool isFlying = true;
    bool isAttacking = false;
    bool isDead = false;
    bool remove = false;

    void start()
    {
        flyTexture.loadFromFile("enemies/boss flying.png");
        shape.setScale(3.5, 3.5);
        shape.setPosition(400,200);
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
} boss;
void StartEnemies()
{
    ball_de_voux.start();
    jamminger.start();
    gunvolt.start();
    boss.start();
}
void UpdateEnemies(float deltaTime)
{
    boss.update();
    cout << "boss position: " << boss.shape.getPosition().x << " " <<boss.shape.getPosition().y << endl;
    ball_de_voux.update(deltaTime);
    jamminger.update();
    gunvolt.update();
}
void DrawEnemies(RenderWindow &window)
{
    if (!ball_de_voux.isDead)
        window.draw(ball_de_voux.shape);
    if (!jamminger.isDead)
        window.draw(jamminger.shape);
    if (!gunvolt.isDead)
        window.draw(gunvolt.shape);
    if (!boss.remove)
        window.draw(boss.shape);
}
