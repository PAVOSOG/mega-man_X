#include <SFML/Graphics.hpp>

using namespace sf;

enum Screens
{
    MainMenu,
    Pause,
    GameOver,
    GamePlay1,
    GamePlay2,
    Victory,
};

void StartMainScreen(RenderWindow &window);
void DrawMainScreen(RenderWindow &window);
void UpdateMainScreen(RenderWindow &window, Event &event, Screens &screen);
