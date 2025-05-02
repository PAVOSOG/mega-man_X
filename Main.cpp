#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include "enemies.hpp"

using namespace std;
using namespace sf;

RenderWindow window(sf::VideoMode(1680, 1050), "Mega Man");
Texture Bg;
Texture levelBg;
Texture level2Bg;
Sprite level;
Sprite level2;
Sprite background;
View camera1(Vector2f(0, 0), Vector2f(window.getSize().x, window.getSize().y));
float gameOverTimer=0.0;
void Update();
void Draw();
void Start();
void UpdateGamePlay();
Screens screen = Screens::MainMenu;

Font stopfont;
float deltaTime;
float deltaTimeEnemy;
Music music;
Music music_game_over;

int main() {
    Start();
    Clock clock;
    Clock EnemyClock;
    Image Icon;
    Icon.loadFromFile("playerAnimations/icon.png");
    window.setIcon(Icon.getSize().x, Icon.getSize().y, Icon.getPixelsPtr());
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();
        deltaTimeEnemy = EnemyClock.restart().asSeconds();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::C)
                player.canShoot = true; // Use player object
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::Escape) {
                if (screen == Screens::GamePlay1) {
                    screen = Screens::Pause;
                } else if (screen == Screens::Pause) {
                    screen = Screens::GamePlay1;
                }
            }
            if (screen == Screens::MainMenu)
            UpdateMainScreen(window, event,screen); 
        }
        Update();
        Draw();
    }
    return 0;
}

void Start() {
    window.setFramerateLimit(60);
    Bg.loadFromFile("playerAnimations/background1.png");
    levelBg.loadFromFile("playerAnimations/foreground1.png");
    level2Bg.loadFromFile("playerAnimations/foreground2.png");
    music.openFromFile("playerAnimations/bg music.mp3");
    music_game_over.openFromFile("playerAnimations/gameover.mp3");
    stopfont.loadFromFile("playerAnimations/main-menu-font.otf");

    background.setScale(5, 5);
    background.setPosition(0, 0);
    background.setTexture(Bg);
    level.setScale(3, 3);
    level.setPosition(0, 0);
    level.setTexture(levelBg);

    level2.setScale(3, 3);
    level2.setPosition(0, 0);
    level2.setTexture(level2Bg);

    music.setLoop(true);
    music.setVolume(3);
    music.play();

    player.start(); 
    StartMainScreen(window);
}

void UpdateGamePlay() {
    std::cout << player.getPosition().x << " " << player.getPosition().y << endl;
    window.setView(camera1);
    camera1.setCenter({ player.getPosition().x, (float)window.getSize().y / 2 });
    player.update(deltaTime, camera1,screen);
    UpdateEnemies(deltaTimeEnemy, screen);
    if (player.isDead)
        screen = Screens::GameOver;
}

void Update() {
    if (screen == Screens::GamePlay1||screen==Screens::GamePlay2) {
        UpdateGamePlay();
       
    } 
}

void DrawGamePlay() {
    window.clear();
    window.draw(background);
    if (screen == Screens::GamePlay1) {
        window.draw(level);
    } else if (screen == Screens::GamePlay2) {
        window.draw(level2);
    }
    player.draw(window); 
    DrawEnemies(window, screen);
}

void DrawGameOver() {
    DrawGamePlay(); // Draw the game elements behind the pause menu
    RectangleShape pauseMenu(Vector2f(window.getSize()));
    pauseMenu.setFillColor(Color(0, 0, 0, 128));
    window.draw(pauseMenu);
    Text pauseText("Game Over", stopfont, 72);
    pauseText.setFillColor(Color::White);
    pauseText.setPosition(camera1.getCenter().x - pauseText.getGlobalBounds().width / 2,
                          camera1.getCenter().y - pauseText.getGlobalBounds().height / 2);
    pauseMenu.setPosition(camera1.getCenter().x - pauseText.getGlobalBounds().width / 2,
                          camera1.getCenter().y - pauseText.getGlobalBounds().height / 2);
    window.draw(pauseText);
    music.stop();
    music_game_over.play();
    gameOverTimer+=deltaTime;
    if (gameOverTimer>=2)
    {
        window.close();
    }
}
void DrawPauseMenu() {
    DrawGamePlay(); // Draw the game elements behind the pause menu
    RectangleShape pauseMenu(Vector2f(window.getSize()));
    pauseMenu.setFillColor(Color(0, 0, 0, 128));
    window.draw(pauseMenu);
    Text pauseText("PAUSED", stopfont, 72);
    pauseText.setFillColor(Color::White);
    pauseText.setPosition(camera1.getCenter().x - pauseText.getGlobalBounds().width / 2,
                          camera1.getCenter().y - pauseText.getGlobalBounds().height / 2);
    pauseMenu.setPosition(camera1.getCenter().x - pauseText.getGlobalBounds().width / 2,
                          camera1.getCenter().y - pauseText.getGlobalBounds().height / 2);
    window.draw(pauseText);  
}


void DrawVictory() {
    DrawGamePlay();
    
    RectangleShape overlay(Vector2f(window.getSize()));
    overlay.setFillColor(Color(0, 128, 0, 128));
    window.draw(overlay);
    
    Text victoryText("Victory!", stopfont, 72);
    victoryText.setFillColor(Color::White);
    victoryText.setPosition(camera1.getCenter().x - victoryText.getGlobalBounds().width / 2,
                              camera1.getCenter().y - victoryText.getGlobalBounds().height / 2);
    window.draw(victoryText);
}
void Draw() {
    window.clear();
    cout << "Screen: " << (screen) << endl;
    cout << "Player Position: " << player.getPosition().x << ", " << player.getPosition().y << endl;
    cout << "Player Health: " << player.health << endl;
    switch(screen) {
        case Screens::GamePlay1:
        case Screens::GamePlay2:
            DrawGamePlay();
            break;
        case Screens::Pause:
            DrawPauseMenu();
            break;
        case Screens::MainMenu:
            DrawMainScreen(window);
            break;
        case Screens::GameOver:
            DrawGameOver();
            break;
        case Screens::Victory:
            DrawVictory();
            break;
    }

    window.display();
}
