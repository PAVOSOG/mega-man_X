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
struct BDV
{
    Sprite shape;
    Texture walk; 
    int col = 0;
    float bdvmovetimer = 0;
    bool isDead = false;
    int health = 4;
    int startX = 0;
    int startY = 0;
    int damage = 1;
    void start(int x, int y)
    {
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
            if (ball_de_voux.shape.getPosition().x > 252)
                shape.move(-2, 0);

            
        }
    }
} ball_de_voux;
struct Jamminger
{
    Sprite shape;
    Texture attack;
    Vector2f velocity;
    int damage = 3;
    int yCord;
    int speed = 1;
    int col = 0;
    bool isDead = false;
    int health = 4;
    int startX = 0, startY = 0;
    void start(int x, int y)
    {
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
} jamminger;
struct GunVolt
{
    Sprite shape;
    Texture gettingReady;
    Texture attack;
    int damage = 3;
    int col = 0;
    bool isDead = false;
    int health = 4;
    void start(int x, int y)
    {
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
    int health = 4;
    bool remove = false;
    int damage = 3;

    void start(int x, int y)
    {
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
} boss;
void StartEnemies()
{
    ball_de_voux.start(2400, 200);
    jamminger.start(3945, 0);
    gunvolt.start(6790, 200);
    // boss.start(500, 200);
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
void checkPlayerHealth(int damage, Sprite shape)
{
    if (player.isImmune)
    {
        return;
    } 
    //cout << player.health << endl;
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
    damageTimer=0;
}
void UpdateEnemies(float deltaTime)
{   
    damageTimer+=deltaTime;
    if (damageTimeframe<=damageTimer)  
    {
        player.isImmune = false;
    }
    checkPlayerHealth(ball_de_voux.damage,ball_de_voux.shape);
    checkPlayerHealth(jamminger.damage, jamminger.shape);
    checkPlayerHealth(gunvolt.damage, gunvolt.shape);
    // checkPayerHealth(boss.damage, boss.shape);

    for (int i = 0; i < player.bullets.size(); i++)
    {
        if (updateEnemiesColor(i, ball_de_voux.health, ball_de_voux.shape.getPosition(), ball_de_voux.isDead, ball_de_voux.shape))
            continue;
        if (updateEnemiesColor(i, jamminger.health, jamminger.shape.getPosition(), jamminger.isDead, jamminger.shape))
            continue;
        if (updateEnemiesColor(i, gunvolt.health, gunvolt.shape.getPosition(), gunvolt.isDead, gunvolt.shape))
            continue;
        // if (updateEnemiesColor(i, boss.health, boss.shape.getPosition(), boss.isDead, boss.shape))
        //     continue;
    }
    boss.update();
    cout<<damageTimer<<"\n";
    //cout << "boss position: " << boss.shape.getPosition().x << " " << boss.shape.getPosition().y << endl;
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
    // if (!boss.remove)
    //     window.draw(boss.shape);
}
