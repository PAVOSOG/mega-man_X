#include "screen.hpp"
#include <string>
using namespace sf;

Font font;
const int menu_items = 4;


std::string element[menu_items] = {"Level one", "Level two","About", "Exit"};
Text menu_text[menu_items];
int selected_index = 0;
Sprite bgSprite;
Texture bgTexture;
std::string about = R"(In the year 21XX, humans live alongside intelligent robots known as Reploids, 
created based on the design of a highly advanced robot named X. X was discovered in a capsule by
Dr. Cain, a scientist who accidentally awakened him after he had been sealed away by his creator,
Dr. Light, for testing to ensure he wouldn't harm humans. Dr. Light had warned that X's free will
and emotions could be dangerous if misused.Ignoring the warning, Dr. Cain used X's design to create 
Reploids, capable of independent thought and emotion. Eventually, some Reploids began to go rogue these
are  called Mavericks. To deal with them, a special unit was formed: The Maverick Hunters.
One of their top leaders, Sigma, became corrupted and turned against humanity, starting a rebellion.
Now, X must fight alongside other Hunters like Zero to stop Sigma and his Maverick army and to decide
his own destiny.

                                 PRESS ESCAPE TO GO BACK!!)";


Text aboutText;
bool aboutScreen=0;
Font arial_font;

void StartMainScreen(RenderWindow &window)
{

    bgTexture.loadFromFile("playerAnimations/MainMenuBackground.jpg");
    bgSprite.setTexture(bgTexture);
    bgSprite.setScale(2, 2);
    font.loadFromFile("playerAnimations/main-menu-font.otf");
    arial_font.loadFromFile("playerAnimations/arial.ttf");
    aboutText.setFont(arial_font);
    aboutText.setCharacterSize(25);
    aboutText.setLetterSpacing(2);
    aboutText.setLineSpacing(2);
    aboutText.setString(about);
    aboutText.setFillColor(Color::White);

}

void UpdateMainScreen(RenderWindow &window, Event &event, Screens &screen, Music& l1, Music& l2)
{

    if (event.type == Event::KeyPressed)
    {
        if(event.key.code==Keyboard::Escape&&aboutScreen)
        {
            aboutScreen=false;
        }
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
            else if (selected_index == 3)
            {
                window.close();
            }
            else if(selected_index==2)
            {
                aboutScreen = true;
            }
        }
    }
}

void DrawAboutBox(RenderWindow &window)
{
    RectangleShape box;
    box.setSize(Vector2f(window.getSize()-Vector2u(200,375)));
    box.setPosition(100,100);
    box.setFillColor(Color(0, 0, 0, 200));
    box.setOutlineThickness(3);
    box.setOutlineColor(Color::White);

    aboutText.setPosition(120.f, 120.f);

    window.draw(box);
    window.draw(aboutText);
    
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
    if (aboutScreen){
        DrawAboutBox(window);
    }
}
