#include "player.hpp"

using namespace sf;
//zoomin
const float IDLE_FRAME_TIME = 0.2f;
const float WALK_FRAME_TIME = 0.1f;
const float JUMP_FRAME_TIME = 0.15f;
const float SPAWN_DURATION = 1.5f;
const float SPAWN_FRAME_TIME = 0.03f;
const float SHOOTING_DURATION = 0.3f;
const float BULLET_SPEED = 1800.0f;
const float DEATH_FRAME_TIME = 0.1f;
const float HIT_FRAME_TIME = 0.4f;
Player player;

void Player::start() {
    ismoving = false;
    isShooting = false;
    isjumping = false;
    isSpawning = true;
    canShoot = true;
    isHit = false;
    health = 6;
    isDead = false;
    playerDirection = Direction::Right;
    animationIndex = 0;
    hitAnimationTimer = 0.0f;
    idleAnimationTimer = 0.0f;
    walkAnimationTimer = 0.0f;
    jumpAnimationTimer = 0.0f;
    shootAnimationTimer = 0.0f;
    deathAnimationTimer = 0.0f;
    spawnTimer = 0.0f;
    shootingTimer = 0.0f;
    isImmune = false;
    speedY = 0.0f;
    playersize = 3;
    playerSprite.setPosition(250,315);
    loadTextures();
    init();
}

void Player::loadTextures() {
    idleTexture.loadFromFile("playerAnimations/mega_idle.png");
    walkTexture.loadFromFile("playerAnimations/mega_walk.png");
    jumpTexture.loadFromFile("playerAnimations/mega_jump.png");
    shootIdleTexture.loadFromFile("playerAnimations/mega_shot.png");
    shootWalkTexture.loadFromFile("playerAnimations/mega_walk_shot.png");
    shootJumpTexture.loadFromFile("playerAnimations/mega_jump_shot.png");
    bulletTexture.loadFromFile("playerAnimations/blast.png");
    spawnTexture.loadFromFile("playerAnimations/mega_spawn.png");
    deathTexture.loadFromFile("playerAnimations/mega_death.png");
}

void Player::init() {
    playerSprite.setOrigin(51 / 2.0, 51 / 2.0);
    playerSprite.setScale(playersize, playersize);
    playerSprite.setPosition({0, (-51 / 2.0)});
    playerSprite.setTexture(spawnTexture);
    playerSprite.setTextureRect(IntRect(0, 0, 51, 51));
}

void Player::update(float& deltaTime, View &view, Screens &screen) {
    if (isSpawning) {
        spawnAnimation(deltaTime);
    } else {
        playerMouvement(deltaTime, screen, view);
        shooting();
        playerAnimation(deltaTime);
        updateBullets(deltaTime, view);
    }
}

void Player::playerMouvement(float& deltaTime, Screens &screen, View &view) {
    ismoving = false;
    if (!isHit && Keyboard::isKeyPressed(Keyboard::Right)&& playerSprite.getPosition().x<=8490) {
        playerSprite.move(5, 0);
        ismoving = true;
        playerSprite.setScale(playersize, playersize);
        playerDirection = Right;
    }
    if (!isHit && Keyboard::isKeyPressed(Keyboard::Left)&& playerSprite.getPosition().x>=255) {
        playerSprite.move(-5, 0);
        ismoving = true;
        playerSprite.setScale(-playersize, playersize);
        playerDirection = Left;
    }
    
    float floor = 315;
    playerSprite.move(0, speedY);
    if (playerSprite.getPosition().x >= 2400) floor = 1000;
    if (playerSprite.getPosition().x >= 2525) floor = 220;

    if (playerSprite.getPosition().x >= 3945) floor = 2300;

    if (playerSprite.getPosition().x >= 4045) floor = 255;

    if (playerSprite.getPosition().x >= 7030) floor = 2300;

    if (playerSprite.getPosition().x >= 7140) floor = 255;
    if (playerSprite.getPosition().x >= 7130 && playerSprite.getPosition().x <= 7030 && playerSprite.getPosition().y > 320 || playerSprite.getPosition().x >= 3945 && playerSprite.getPosition().x <= 4045 && playerSprite.getPosition().y > 320 || playerSprite.getPosition().y > 380 && playerSprite.getPosition().x >= 2430 && playerSprite.getPosition().x <= 2525) {
       screen = Screens::GameOver;
    }
   

    if (playerSprite.getPosition().y >= floor) {
        playerSprite.setPosition(playerSprite.getPosition().x, floor);
        speedY = 0;
        isjumping = false;
        if (!isHit && Keyboard::isKeyPressed(Keyboard::X)) {
            speedY = -20;
            isjumping = true;
        }
    } else {
        speedY += 1;
    }}

void Player::draw(RenderWindow &window) {
    window.draw(playerSprite);
    for (const auto &bullet : bullets) {
        window.draw(bullet.shape);
    }
}


void Player::playerAnimation(float& deltaTime) {
    static bool wasMoving = false;
    static bool wasJumping = false;
    if (isHit) {
        playerSprite.setTexture(deathTexture);
        playerSprite.setTextureRect(IntRect(5 * 51, 0, 51, 51));
        hitAnimationTimer += deltaTime;
        if (hitAnimationTimer >= HIT_FRAME_TIME) {
            isHit = false;
            hitAnimationTimer = 0.0f;
        }
    }
    else {
        if (ismoving != wasMoving || isjumping != wasJumping) {
            animationIndex = 0;
            wasMoving = ismoving;
            wasJumping = isjumping;
        }
        if (isShooting) {
            if (isjumping) {
                if (speedY < 0) {
                    if (animationIndex > 2) {
                        animationIndex = 2;
                    }
                }
                else {
                    if (animationIndex != 3) {
                        animationIndex = 3;
                    }

                }

                playerSprite.setTexture(shootJumpTexture);
                playerSprite.setTextureRect(IntRect(animationIndex * 51, 0, 51, 51));
                animationIndex = ((animationIndex + 1) / 3) % 7;
            }
            else if (ismoving) {
                walkAnimationTimer += deltaTime;
                if (walkAnimationTimer >= WALK_FRAME_TIME) {
                    walkAnimationTimer = 0.0f;
                    playerSprite.setTexture(shootWalkTexture);
                    playerSprite.setTextureRect(IntRect(animationIndex * 51, 0, 51, 51));
                    animationIndex = (animationIndex + 1) % 10;
                }

            }
            else {
                playerSprite.setTexture(shootIdleTexture);
                playerSprite.setTextureRect(IntRect(animationIndex * 51, 0, 51, 51));
                animationIndex = (animationIndex + 1) % 2;

            }
            shootingTimer += deltaTime;
            if (shootingTimer >= SHOOTING_DURATION) {
                isShooting = false;
                shootingTimer = 0.0f;
            }
        }
        else {
            if (isjumping) {
                jumpAnimationTimer += deltaTime;
                if (jumpAnimationTimer >= JUMP_FRAME_TIME) {
                    jumpAnimationTimer = 0.0f;
                }
                if (speedY < 0) {
                    if (animationIndex > 2) {
                        animationIndex = 2;
                    }
                }
                else {
                    if (animationIndex != 3) {
                        animationIndex = 3;
                    }

                }

                playerSprite.setTexture(jumpTexture);
                playerSprite.setTextureRect(IntRect(animationIndex * 51, 0, 51, 51));
                animationIndex = ((animationIndex + 1) / 3) % 7;
            }
            else if (ismoving) {
                walkAnimationTimer += deltaTime;
                if (walkAnimationTimer >= WALK_FRAME_TIME) {
                    walkAnimationTimer = 0.0f;
                    playerSprite.setTexture(walkTexture);
                    playerSprite.setTextureRect(IntRect(animationIndex * 51, 0, 51, 51));
                    animationIndex = (animationIndex + 1) % 11;
                }
            }


            else {
                idleAnimationTimer += deltaTime;
                if (idleAnimationTimer >= IDLE_FRAME_TIME) {
                    idleAnimationTimer = 0.0f;
                    playerSprite.setTexture(idleTexture);
                    playerSprite.setTextureRect(IntRect(animationIndex * 51, 0, 51, 51));
                    animationIndex = (animationIndex + 1) % 4;
                }
            }
        }
        // Reset timers when animation state changes
        if (!ismoving && !isjumping) {
            walkAnimationTimer = 0.0f;
            jumpAnimationTimer = 0.0f;
        }
        if (!isjumping) {
            jumpAnimationTimer = 0.0f;
        }
        if (isDead) {
            deathAnimationTimer += deltaTime;
            if (deathAnimationTimer >= DEATH_FRAME_TIME) {
                deathAnimationTimer = 0.0f;
                playerSprite.setTexture(deathTexture);
                playerSprite.setTextureRect(IntRect(animationIndex * 51, 0, 51, 51));
                animationIndex = (animationIndex + 1) % 12;
            }
        }
    }
}

void Player::spawnAnimation(float& deltaTime) {
    playerSprite.move(0, 50);
    if (playerSprite.getPosition().y >= 315) {
        playerSprite.setPosition(250, 315);
        spawnTimer += deltaTime;
        if (spawnTimer >= SPAWN_FRAME_TIME) {
            spawnTimer = 0.0f;
            if (animationIndex >= 6) {
                idleAnimationTimer = IDLE_FRAME_TIME;
                animationIndex = 0;
                isSpawning = false;
                playerSprite.setTexture(idleTexture);
                return;
            }
            playerSprite.setTextureRect(IntRect(animationIndex * 51, 0, 51, 51));
            animationIndex = (animationIndex + 1) % 7;
        }
    }
}

void Player::shooting() {
    if (!isHit && Keyboard::isKeyPressed(Keyboard::C) && canShoot) {
        shoot(player.getPosition());
        canShoot = false;
        isShooting = true;
        shootingTimer = 0.0f;
    }
}
void Player::shoot(Vector2f playerPosition) {
    Bullet bullet;
    bullet.shape.setTexture(bulletTexture);
    bullet.shape.setPosition(player.getPosition());
    float bulletspeed = 1800.0f;
    bullet.velocity = (playerDirection == Right) ? Vector2f(bulletspeed, 0) : Vector2f(-bulletspeed, 0);
    bullets.push_back(bullet);
}


void Player::updateBullets(float& deltaTime, const View &view) {
    for (size_t i = 0; i < bullets.size(); i++) {
        bullets[i].shape.move(bullets[i].velocity * deltaTime);

        if (bullets[i].shape.getPosition().x +
                    bullets[i].shape.getGlobalBounds().width <
                view.getCenter().x - view.getSize().x / 2 ||
            bullets[i].shape.getPosition().x >
                view.getCenter().x + view.getSize().x / 2) {
            bullets.erase(bullets.begin() + i);
        }
    }
}

Vector2f Player::getPosition() const { return playerSprite.getPosition(); }

Sprite &Player::getSprite() { return playerSprite; }