#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;

struct Bullet {
    Sprite shape;
    Vector2f velocity;
};

enum Direction { Right, Left };

struct Player {
    bool ismoving;
    bool isShooting;
    bool isjumping;
    bool isSpawning;
    bool canShoot;
    Direction playerDirection;
    int animationIndex;
    float idleAnimationTimer;
    float walkAnimationTimer;
    float jumpAnimationTimer;
    float shootAnimationTimer;
    float spawnTimer;
    float shootingTimer;
    float speedY;
    int playersize;
    Sprite playerSprite;
    Texture idleTexture;
    Texture walkTexture;
    Texture jumpTexture;
    Texture shootIdleTexture;
    Texture shootWalkTexture;
    Texture shootJumpTexture;
    Texture bulletTexture;
    Texture spawnTexture;
    std::vector<Bullet> bullets;

    void start();
    void loadTextures();
    void init();
    void update(float& deltaTime, const View &view);
    void draw(RenderWindow &window);
    void playerMouvement(float& deltaTime);
    void playerAnimation(float& deltaTime);
    void spawnAnimation(float& deltaTime);
    void shoot(Vector2f playerPosition);
    void shooting();
    void updateBullets(float& deltaTime, const View &view);
    Vector2f getPosition() const;
    Sprite &getSprite();
};

extern Player player;
