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

struct BoomBeen
{
    Sprite shape;
    Texture attack;
    Vector2f velocity;
    int yCord;
    int speed = 1;
    int col = 0;
    bool isDead = false;
    int health = 2;
    int damage = 2;
    void start(int x, int y)
    {
        attack.loadFromFile("enemies/gun volt1.png");
        shape.setScale(4, 4);
        yCord = rand() % 400;
        shape.setPosition(Vector2f(x, y));
    }
    void movement()
    {
        velocity = (player.getPosition()) - (shape.getPosition());
        float mag = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
        velocity = {velocity.x / mag, velocity.y / mag};
        velocity = {velocity.x * speed, velocity.y * speed};
        shape.move(velocity);
    }
    void update()
    {
        shape.setTexture(attack);
        Scaling();
        movement();
        shape.setTextureRect(IntRect(col * 48, 0, 48, 50));
        col = (col + 1) % 9;
    }
    void Scaling()
    {
        if (shape.getPosition().x < player.getPosition().x)
        {
            shape.setScale(-4, 4);
            shape.move(1, 0);
        }
        else
        {
            shape.setScale(4, 4);
            shape.move(-1, 0);
        }
    }
};

struct BOSS
{
    int speed = 1;
    Sprite shape;
    Sprite missileBoss;
    Texture moka3ab;
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
    Vector2f bossmisspos, velocity;
    float flyFrame = 0.05f;
    float fly_timer = 0;
    bool switched = false;

    void start(int x, int y, float deltaTime)
    {
        moka3ab.create(100, 100);
        missileBoss.setTexture(moka3ab);
        missileBoss.setColor(sf::Color::Green);
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
        fly(deltaTime);
        missileBoss.setPosition(shape.getPosition().x + 100, shape.getPosition().y);
    }
    void movement()
    {
        velocity = {player.getPosition().x - shape.getPosition().x, 0};
        float mag = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
        if (mag != 0)
        {
            velocity.x = (velocity.x / mag) * speed;
            velocity.y = (velocity.y / mag) * speed;
        }
        shape.move(velocity);
    }
    void update(float deltaTime)
    {
        missileBoss.setTexture(moka3ab);
        missileBoss.setColor(sf::Color::Green);
        if (isFlying)
        {
            flyMovement();
            fly(deltaTime);
        }
        else if (isAttacking)
        {
            attack(deltaTime);
            movement();
        }
        if (isDead)
            dead();
        missileBoss.setPosition(shape.getPosition().x + 50, shape.getPosition().y + 350);
        if (player.getPosition().x+200 > shape.getPosition().x && !switched)
        {
            shape.setScale(-3.5,3.5);
            switched = true;
        }
        else if (player.getPosition().x+200 < shape.getPosition().x && switched)
        {
            switched = false;
            shape.setScale(3.5, 3.5);
        }
    }
    void fly(float delTatime)
    {
        fly_timer += delTatime;
        if (fly_timer >= flyFrame)
        {
            shape.setTexture(flyTexture);
            shape.setTextureRect(IntRect(col * 148, 0, 148, 120));
            col = (col + 1) % 3;
            fly_timer = 0;
        }
        if (shape.getPosition().y >= 0)
        {
            isFlying = false;
            isAttacking = true;
        }
    }
    void attack(float deltaTime)
    {
        //     col = 0;
        fly_timer += deltaTime;
        if (fly_timer >= flyFrame)
        {
            attackTexture.loadFromFile("enemies/boss attacking.png");
            shape.setTexture(attackTexture);
            shape.setTextureRect(IntRect(col * 148, row * 128, 148, 128));
            col = (col + 1) % 3;
            row = (row + 1) % 2;
            fly_timer = 0;
        }
    }
    void dead()
    {
        deadTexture.loadFromFile("enemies/boss dead.png");
        shape.setTexture(deadTexture);
        shape.setTextureRect(IntRect(0, 3 * 112, 148, 112));
        shape.move(0, 0.5);
        if (shape.getPosition().y >= -50)
        {
            remove = true;
        }
    }
    void flyMovement()
    {
        if (shape.getPosition().y < -0)
        {
            shape.move(0, 7);
        }
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
Vector2f ball_de_vouxPosV2[ball_de_vouxCountV2] = {{2200, 200}, {1400, 200}, {6400, 200}, {7400, 200}, {7900, 200}};

BoomBeen boomBeen[4];
const int bombBeenCountV1 = 3;
const int boomBeenCountV2 = 4;
Vector2f boomBenPosV1[bombBeenCountV1] = {{8400, 100}, {1500, 100}, {5000, 100}};
Vector2f boomBenPosV2[boomBeenCountV2] = {{2400, 100}, {1400, 100}, {2400, 100}, {6000, 100}};

BOSS boss;
Vector2f bossPos = {500, -1300};

//////////////////////////////////////////////////////////////////////////////////////////////////

int ball_de_vouxCount;
int jammingerCount;
int gunvoltCount;

void StartEnemies(Screens screen, float deltaTime)
{
    if (screen == Screens::GamePlay1)
    {
        ball_de_vouxCount = ball_de_vouxCountV1;
        jammingerCount = jammingerCountV1;
        gunvoltCount = bombBeenCountV1;

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
            boomBeen[i].start(boomBenPosV1[i].x, boomBenPosV1[i].y);
        }
    }
    else if (screen == Screens::GamePlay2)
    {
        ball_de_vouxCount = ball_de_vouxCountV2;
        jammingerCount = jammingerCountV2;
        gunvoltCount = boomBeenCountV2;
        boss.start(bossPos.x, bossPos.y, deltaTime);
        for (int i = 0; i < ball_de_vouxCount; i++)
        {
            ball_de_voux[i].start(ball_de_vouxPosV2[i].x, ball_de_vouxPosV2[i].y);
        }
        for (int i = 0; i < jammingerCount; i++)
        {
            jamminger[i].start(jammingerPosV2[i].x, jammingerPosV2[i].y);
        }
        for (int i = 0; i < gunvoltCount; i++)
        {
            boomBeen[i].start(boomBenPosV2[i].x, boomBenPosV2[i].y);
        }
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
    bool isWon = true;
    if (oldScreen != screen)
    {
        oldScreen = screen;
        StartEnemies(screen, deltaTime);
    }

    damageTimer += deltaTime;
    if (damageTimeframe <= damageTimer)
    {
        player.isImmune = false;
    }

    for (int i = 0; i < ball_de_vouxCount; i++)
    {
        checkPlayerHealth(ball_de_voux[i].damage, ball_de_voux[i].shape, ball_de_voux[i].isDead);
        cout << "Ball De Voux " << i << endl;
        cout << ball_de_voux[i].isDead << i << endl;
        if (!ball_de_voux[i].isDead)
            isWon = false;
        ball_de_voux[i].update(deltaTime);
    }
    for (int i = 0; i < jammingerCount; i++)
    {
        if (!jamminger[i].isDead)
            isWon = false;
        cout << "Jamminger " << i << endl;
        cout << jamminger[i].isDead << i << endl;
        checkPlayerHealth(jamminger[i].damage, jamminger[i].shape, jamminger[i].isDead);
        jamminger[i].update();
    }
    for (int i = 0; i < gunvoltCount; i++)
    {
        cout << "BoomBeen " << i << endl;
        cout << boomBeen[i].isDead << i << endl;
        if (!boomBeen[i].isDead)
            isWon = false;
        checkPlayerHealth(boomBeen[i].damage, boomBeen[i].shape, boomBeen[i].isDead);
        boomBeen[i].update();
    }
    if (screen == Screens::GamePlay2)
    { // boss fight
        cout << "Boss" << endl;
        cout << boss.isDead << endl;
        if (!boss.isDead)
            isWon = false;
        checkPlayerHealth(boss.damage, boss.missileBoss, boss.isDead);
        boss.update(deltaTime);
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

        // BoomBeen
        for (int i = 0; i < gunvoltCount; i++)
        {
            shouldBreak = updateEnemiesColor(j, boomBeen[i].health, boomBeen[i].shape.getPosition(), boomBeen[i].isDead, boomBeen[i].shape);
            if (shouldBreak)
                break;
        }
        if (shouldBreak)
            continue;

        // Boss
        if (screen == Screens::GamePlay2 && updateEnemiesColor(j, boss.health, boss.shape.getPosition(), boss.isDead, boss.shape))
            continue;
    }
    if (isWon)
    {
        screen = Screens::Victory;
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
        if (!boomBeen[i].isDead)
            window.draw(boomBeen[i].shape);
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
    {
        // if(boss.shape.getposition().y<)
        window.draw(boss.shape);

        window.draw(boss.missileBoss);
    }
}
