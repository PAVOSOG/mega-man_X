#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include "enemies.hpp"
#include "player.hpp" 

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

enum Screens {
    MainMenu, Pause, EndScreen, GamePlay1, GamePlay2
};
void Update();
void Draw();
void Start();
void UpdateGamePlay1();
Screens screen = GamePlay1;

float deltaTime;
float deltaTimeEnemy;
Music music;

int main() {
    Start();
    Clock clock;
    Clock EnemyClock;
    
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

    background.setScale(5, 5);
    background.setPosition(0, 0);
    background.setTexture(Bg);
    level.setScale(3, 3);
    level.setPosition(0, 0);
    level.setTexture(levelBg);

    level2.setScale(500, 500);
    level2.setPosition(0, 0);
    level2.setTexture(level2Bg);

    music.setLoop(true);
    music.setVolume(3);
    music.play();

    player.start(); 
    StartEnemies();
}

void UpdateGamePlay1() {
    std::cout << player.getPosition().x << " " << player.getPosition().y << endl;
    window.setView(camera1);
    camera1.setCenter({ player.getPosition().x, (float)window.getSize().y / 2 });

    player.update(deltaTime, camera1); 

    UpdateEnemies(deltaTimeEnemy);
}

void Update() {
    if (screen == Screens::GamePlay1) {
        UpdateGamePlay1();
    }
}

void DrawGamePlay1() {
    window.clear();
    window.draw(background);
    if (screen == Screens::GamePlay1) {
        window.draw(level);
    } else if (screen == Screens::GamePlay2) {
        window.draw(level2);
    }
    player.draw(window); 
    DrawEnemies(window);
}

void DrawPauseMenu() {
    DrawGamePlay1(); // Draw the game elements behind the pause menu
    RectangleShape pauseMenu(Vector2f(window.getSize()));
    pauseMenu.setFillColor(Color(0, 0, 0, 128));
    window.draw(pauseMenu);
    Font font;
    font.loadFromFile("playerAnimations/main-menu-font.otf");
    Text pauseText("PAUSED", font, 72);
    pauseText.setFillColor(Color::White);
    pauseText.setPosition(camera1.getCenter().x - pauseText.getGlobalBounds().width / 2,
                          camera1.getCenter().y - pauseText.getGlobalBounds().height / 2);
    pauseMenu.setPosition(camera1.getCenter().x - pauseText.getGlobalBounds().width / 2,
                          camera1.getCenter().y - pauseText.getGlobalBounds().height / 2);
    window.draw(pauseText);  
}

void Draw() {
    window.clear();
    switch(screen) {
        case Screens::GamePlay1:
            DrawGamePlay1();
            break;
        case Screens::Pause:
            DrawPauseMenu();
            break;
    }

    window.display();
}
