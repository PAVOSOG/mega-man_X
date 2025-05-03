#include "screen.hpp"
#include <string>
using namespace sf;

Font font;
const int menu_items = 3;

std::string element[menu_items] = {"Level one", "Level two", "Exit"};
Text menu_text[menu_items];
int selected_index = 0;
Sprite bgSprite;
Texture bgTexture;

void StartMainScreen(RenderWindow &window)
{

    bgTexture.loadFromFile("playerAnimations/MainMenuBackground.jpg");
    bgSprite.setTexture(bgTexture);
    bgSprite.setScale(2, 2);
    bool load_font = font.loadFromFile("playerAnimations/main-menu-font.otf");
}

void UpdateMainScreen(RenderWindow &window, Event &event, Screens &screen, Music& l1, Music& l2)
{

    if (event.type == Event::KeyPressed)
    {
        if (event.key.code == Keyboard::W && selected_index > 0)
        {
            selected_index--;
        }
        else if (event.key.code == Keyboard::S && selected_index < menu_items - 1)
        {

            selected_index++;
        }
        else if (event.key.code == Keyboard::Enter)
        {
            if (selected_index == 0)
            {
                screen = GamePlay1;
                l1.play();
            }
            else if (selected_index == 1)
            {
                screen = GamePlay2;
                l2.play();
            }
            else if (selected_index == 2)
            {
                window.close();
            }
        }
    }
}

void DrawMainScreen(RenderWindow &window)
{
    Vector2i position = {0, 0};
    window.draw(bgSprite);
    bgSprite.setPosition(0, 0);
    window.setPosition(position);
    for (int i = 0; i < menu_items; i++)
    {
        menu_text[i].setFont(font);
        menu_text[i].setString(element[i]);
        menu_text[i].setCharacterSize(48);
        menu_text[i].setPosition(50.f, 150.f + i * 75);
        menu_text[i].setFillColor(i == selected_index ? Color::Yellow : Color::White);
        window.draw(menu_text[i]);
    }
}
