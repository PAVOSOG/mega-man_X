#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include "enemies.hpp"

using namespace std;
using namespace sf;

RenderWindow window(VideoMode::getDesktopMode(), "Mega Man", Style::Fullscreen);
Texture Bg;
Texture levelBg;
Texture level2Bg;
Sprite HealthBarSprite;
Sprite level;
Sprite level2;
Sprite background;
Texture healthBar;
View camera1(Vector2f(0, 0), Vector2f(window.getSize().x*0.75, window.getSize().y*0.5));
float gameOverTimer=0.0;
void Update();
void Draw();
void Start();
void UpdateGamePlay();
Screens screen = Screens::MainMenu;
Screens oldLevelScreen = Screens::GamePlay1;

Font stopfont;
float deltaTime;
float deltaTimeEnemy;
Music musicLevel2;
Music music;
Music music_game_over;
Music music_victory;

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
                if (screen == Screens::GamePlay1 || screen == Screens::GamePlay2) {
                    oldLevelScreen = screen;
                    screen = Screens::Pause;
                    music.pause();
                    musicLevel2.pause();
                } else if (screen == Screens::Pause) {
                    screen = oldLevelScreen;
                    if (oldLevelScreen == Screens::GamePlay1) {
                        music.play();
                    } else if (oldLevelScreen == Screens::GamePlay2) {
                        musicLevel2.play();
                    }
                }
            }
            if (screen == Screens::MainMenu)
            UpdateMainScreen(window, event,screen, music, musicLevel2); 
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
    music_victory.openFromFile("playerAnimations/victory.mp3");
    musicLevel2.openFromFile("playerAnimations/level2.mp3");
    stopfont.loadFromFile("playerAnimations/main-menu-font.otf");
    healthBar.loadFromFile("playerAnimations/healthBar.png");

    background.setScale(5, 5);
    background.setPosition(0, 0);
    background.setTexture(Bg);
    level.setScale(3, 3);
    level.setPosition(0, 0);
    level.setTexture(levelBg);

    level2.setScale(3, 3);
    level2.setPosition(0, 0);
    level2.setTexture(levelBg);

    music.setLoop(true);
    music.setVolume(50);

    musicLevel2.setLoop(true);
    musicLevel2.setVolume(50);

    HealthBarSprite.setScale(0.75, 0.75);
    HealthBarSprite.setPosition(0, 500);
    HealthBarSprite.setTexture(healthBar);

    player.start(); 
    StartMainScreen(window);

}

void UpdateGamePlay() {
    std::cout << player.getPosition().x << " " << player.getPosition().y << endl;
    window.setView(camera1);
    player.update(deltaTime, camera1, screen);
    camera1.setCenter({ player.getPosition().x> (float)window.getSize().x / 2? player.getPosition().x:window.getSize().x/2, (float)window.getSize().y / 2-250});
    HealthBarSprite.setPosition(camera1.getCenter()-Vector2f(750,100));
    UpdateEnemies(deltaTimeEnemy, screen, music_victory);
    if (player.isDead){
        screen = Screens::GameOver;
        music.stop();
        musicLevel2.stop();
        music_game_over.setVolume(50);
        music_game_over.setLoop(false);
        music_game_over.play();
    }
}

void Update() {
    if (screen == Screens::GamePlay1||screen==Screens::GamePlay2) {
        UpdateGamePlay();
    } 
}

void DrawGamePlay() {
    window.clear();
    if (screen == Screens::GamePlay1) {
        window.draw(background);
        window.draw(level);
    }
    else if (screen == Screens::GamePlay2) {
        // window.draw(background); 
        window.draw(level2);
    }
    window.draw(HealthBarSprite);
    RectangleShape MissingHealth;
    MissingHealth.setFillColor(Color::Black);
    MissingHealth.setPosition(camera1.getCenter() - Vector2f(691, 73));
    MissingHealth.setSize({35,(float(6-player.health)/6*150)});
    window.draw(MissingHealth);
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
    music.pause();
    musicLevel2.pause();
    
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
