#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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
void DrawAboutBox(RenderWindow &Window);
void StartMainScreen(RenderWindow &window);
void DrawMainScreen(RenderWindow &window);
void UpdateMainScreen(RenderWindow &window, Event &event, Screens &screen, Music& l1, Music& l2);
