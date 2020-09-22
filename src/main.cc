#include<iostream>
#include <SFML/Graphics.hpp>
#include "Inputs.hh"
#include "Character.hh"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define GAME_NAME "Roguelike game"
#define TILES1 "assets/sprites/tiles1.png"
#define TILES2 "assets/sprites/tiles2.png"
#define TILES3 "assets/sprites/tiles3.png"
#define SPRITE_SCALE 4.f
#define FPS 144
#define PLAYER_MOVESPEED 0.3f

int main()
{
    //esto es la ventana de tu grafico
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);
    //aqui vas a guardar los eventos dentro de la ventana, eje: teclado, mouse, etc.
    sf::Event event;

    char** maze
        {
            new char*[10]
            {
                new char[13]{'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'},
                new char[13]{'w', 'e', 'r', 'w', 'w', 'r', 'w', 'e', 'r', 'w', 'e', 'r', 'w'},
                new char[13]{'g', 'g', 'g', 's', 'g', 'g', 'g', 's', 'g', 'g', 'g', 'g', 'g'},
                new char[13]{'g', 'g', 'g', 'g', 's', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g'},
                new char[13]{'s', 'g', 'a', 's', 'g', 'g', 'g', 'g', 'g', 's', 'g', 'g', 'g'},
                new char[13]{'g', 'g', 'g', 'g', 'g', 'g', 's', 'g', 'g', 'g', 'g', 'g', 'g'},
                new char[13]{'g', 'g', 'g', 'g', 'g', 'g', 'a', 'g', 's', 'g', 'g', 'g', 'g'},
                new char[13]{'g', 'g', 's', 'g', 'g', 'g', 'a', 'g', 'g', 'g', 'g', 'g', 'g'},
                new char[13]{'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'a', 'g', 'g', 'g'},
                new char[13]{'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g'}
            }
        };

    sf::Texture* tilesTexture1{new sf::Texture()};
    tilesTexture1->loadFromFile(TILES1);
    sf::Texture* tilesTexture2{new sf::Texture()};
    tilesTexture2->loadFromFile(TILES2);
    sf::Texture* tilesTexture3{new sf::Texture()};
    tilesTexture3->loadFromFile(TILES3);

    sf::Sprite* tileGround1{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 1, 16 * 4, 16, 16)))};
    tileGround1->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileGround2{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 1, 16 * 5, 16, 16)))};
    tileGround2->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileGround3{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 2, 16 * 6, 16, 16)))};
    tileGround3->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileWall1{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 1, 16 * 1, 16, 16)))};
    tileWall1->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileWall2{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 1, 16 * 2, 16, 16)))};
    tileWall2->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileWall3{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 2, 16 * 2, 16, 16)))};
    tileWall3->setScale(SPRITE_SCALE, SPRITE_SCALE);
    std::vector<sf::Sprite> mazeSprites;

    for (int i ={}; i < 10; i++)
    {
        for (int j = 0; j < 13; j++)
        {
        char& m = *(*(maze + i) + j);
           switch (m)
           {
           case 'w':
                mazeSprites.push_back(*tileWall1);
                mazeSprites.back().setPosition(64 * j, 64 * i);
               break;
           case 'e':
                mazeSprites.push_back(*tileWall2);
                mazeSprites.back().setPosition(64 * j, 64 * i);
               break;
            case 'r':
                mazeSprites.push_back(*tileWall3);
                mazeSprites.back().setPosition(64 * j, 64 * i);
               break;
            case 'g':
                mazeSprites.push_back(*tileGround1);
                mazeSprites.back().setPosition(64 * j, 64 * i);
               break;
            case 'a':
                mazeSprites.push_back(*tileGround2);
                mazeSprites.back().setPosition(64 * j, 64 * i);
               break;
            case 's':
                mazeSprites.push_back(*tileGround3);
                mazeSprites.back().setPosition(64 * j, 64 * i);
               break;
           default:
               break;
           }
        }
        
    }
    

    sf::Clock* clock{new sf::Clock()};
    float deltaTime{};

    window->setFramerateLimit(FPS);

    
    //Game inputs
    Inputs* inputs{new Inputs()};
    //Textures
   

    //Main player
    Character* character1{new Character(tilesTexture2, 16 * 1, 16 * 5, 16, 16, SPRITE_SCALE, SPRITE_SCALE)};
    character1->SetAnimations(
        new Animation*[2]
        {
            new Animation(5, 0, 5, character1->GetSprite(), 40.f),
            new Animation(6, 0, 5, character1->GetSprite(), 80.f)
        }
    );

    //Animation* idle{new Animation(5, 0, 5, character1->GetSprite(), 40.f)};
    //Animation* run{new Animation(6, 0, 5, character1->GetSprite(), 80.f)};

    /*sf::RectangleShape* boxShape{new sf::RectangleShape(*(new sf::Vector2f(100, 100)))};
    boxShape->setPosition((WINDOW_WIDTH / 2), WINDOW_HEIGHT / 2);
    boxShape->setFillColor(sf::Color::Transparent);
    boxShape->setOutlineColor(sf::Color::Green);
    boxShape->setOutlineThickness(2.f);

    sf::CircleShape* pointShape{new sf::CircleShape(2.f)};
    pointShape->setPosition(boxShape->getPosition());
    pointShape->setFillColor(sf::Color::White);*/

    character1->GetSprite()->setPosition(0, 300);

    //esto es el loop principal, mientras la ventana este abierta, esto se va ejecutar.
    while (window->isOpen())
    {
        //mientras se esten ejecutando eventos dentro de la ventana, esto se va repetir eje: teclado, joystick, mouse, etc
        while (window->pollEvent(event))
        {
            //si el evento fue la acción de cerrar la ventana, entonces termina la aplicación.
            if(event.type == sf::Event::Closed)
            {
                window->close();
            }
        }
        
        Vec2* keyboardAxis{inputs->GetKeyboardAxis()};
        Vec2* joystickAxis{inputs->GetJoystickAxis()};
   

        if(sf::Joystick::isConnected(0))
        {
            character1->GetSprite()->move(joystickAxis->x * deltaTime * PLAYER_MOVESPEED, joystickAxis->y * deltaTime * PLAYER_MOVESPEED);
            character1->FlipSpriteX(joystickAxis->x);
            
            if(std::abs(joystickAxis->x) > 0 || std::abs(joystickAxis->y) > 0)
            {
                //run
                character1->GetAnimation(1)->Play(deltaTime);
            }
            else
            {
                //idle
                character1->GetAnimation(0)->Play(deltaTime);
            }
        }
        else
        {
            character1->GetSprite()->move(keyboardAxis->x * deltaTime * PLAYER_MOVESPEED, keyboardAxis->y * deltaTime * PLAYER_MOVESPEED);
            character1->FlipSpriteX(keyboardAxis->x);

            if(std::abs(keyboardAxis->x) > 0 || std::abs(keyboardAxis->y) > 0)
            {
                //run
                character1->GetAnimation(1)->Play(deltaTime);
            }
            else
            {
                //idle
                character1->GetAnimation(0)->Play(deltaTime);
            }
        }

        //character1->GetAnimation(0)->Play(deltaTime);


        window->clear(*(new sf::Color(150, 100, 0, 255)));//lipiar la pantalla
          for(auto& tile : mazeSprites)
        {
            window->draw(tile);
        }
        //window->draw(tileGround3);
        window->draw(*character1->GetSprite());
        window->display(); //mostrar en pantalla lo que se va dibujar

        sf::Time timeElapsed = clock->getElapsedTime();
        deltaTime = timeElapsed.asMilliseconds();
        clock->restart();

        //std::cout << "delta time: " << deltaTime << std::endl;

        delete keyboardAxis;
        delete joystickAxis;
    }
    
    return 0;
}