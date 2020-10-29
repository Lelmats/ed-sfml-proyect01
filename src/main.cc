#include<iostream>
#include <SFML/Graphics.hpp>
#include<box2d/box2d.h>

#include "Maze.hh"
#include "Inputs.hh"
#include "Character.hh"
#include "Tile.hh"
#include "GameObject.hh"
#include "ContactListener.hh"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define GAME_NAME "Roguelike game"
#define TILES1 "assets/sprites/tiles1.png"
#define TILES2 "assets/sprites/tiles2.png"
#define TILES3 "assets/sprites/tiles3.png"
#define SPRITE_SCALE 4.f
#define FPS 144
#define PLAYER_MOVESPEED 3.0f

int main()
{
    //esto es la ventana de tu grafico
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);
    //aqui vas a guardar los eventos dentro de la ventana, eje: teclado, mouse, etc.
    sf::Event event;
  

    //physics declaration
    b2Vec2* gravity{new b2Vec2(0.f, 0.f)};
    b2World* world{new b2World(*gravity)}; 
    world->SetContactListener(new ContactListener());

    sf::Clock* clock{new sf::Clock()};
    float deltaTime{};

    window->setFramerateLimit(FPS);

    //Game inputs
    Inputs* inputs{new Inputs()};

    sf::Texture* tilesTexture1{new sf::Texture()};
    tilesTexture1->loadFromFile(TILES1);
    sf::Texture* tilesTexture2{new sf::Texture()};
    tilesTexture2->loadFromFile(TILES2);
    sf::Texture* tilesTexture3{new sf::Texture()};
    tilesTexture3->loadFromFile(TILES3);

    const float tileBaseWidth{16 * SPRITE_SCALE};
    const float tileBaseHeight{16 * SPRITE_SCALE};

    //Textures
    /*sf::Sprite* chest{new sf::Sprite(*tilesTexture2, *(new sf::IntRect(16 * 6, 16 * 1, 16, 16)))};
    chest->setScale(SPRITE_SCALE, SPRITE_SCALE);
    chest->setPosition(64 * 11,64 * 1.85);
    Animation* shinychest{new Animation(1,6,13,chest,130)};*/

    sf::Sprite* firelamp{new sf::Sprite(*tilesTexture1, *(new sf::IntRect(16 * 1, 17 * 9, 16, 19)))};
    firelamp->setScale(SPRITE_SCALE, SPRITE_SCALE);
    firelamp->setPosition(64 * 0,50 * 2);
    Animation* lampAnimation{new Animation(8,1,7,firelamp,150)};
    
    sf::Sprite* stairs{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 3, 16 * 6, 16, 16)))};
    stairs->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileGround1{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 1, 16 * 4, 16, 16)))};
    tileGround1->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileGround2{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 1, 16 * 5, 16, 16)))};
    tileGround2->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileGround3{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 2, 16 * 6, 16, 16)))};
    tileGround3->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileWall1_1{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 4, 16 * 3, 16, 16)))};
    tileWall1_1->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileWall1_2{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 4, 16 * 4, 16, 16)))};
    tileWall1_2->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileWall1{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 1, 16 * 1, 16, 16)))};
    tileWall1->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileWall2{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 1, 16 * 2, 16, 16)))};
    tileWall2->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileWall3{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 2, 16 * 2, 16, 16)))};
    tileWall3->setScale(SPRITE_SCALE, SPRITE_SCALE);
    sf::Sprite* tileWall4{new sf::Sprite(*tilesTexture3, *(new sf::IntRect(16 * 4, 16 * 5, 16, 16)))};
    tileWall4->setScale(SPRITE_SCALE, SPRITE_SCALE);

    /*BoxCollider* chestCollider = new BoxCollider(64 * 0,50 * 2, new sf::Color(0, 255, 0, 255), 16, 16,
    new Rigidbody(world, b2BodyType::b2_staticBody, new b2Vec2(32 *22,32 * 3.3), 32, 32, 1, 0, 0),chest);
    
    chestCollider->GetBoxShape()->setScale(SPRITE_SCALE, SPRITE_SCALE);
    chestCollider->GetBoxShape()->setPosition(chest->getPosition());*/

//Maze
    unsigned int N{10}, M{13};
    Maze* maze1{new Maze(N, M, SPRITE_SCALE, 16, tilesTexture3, "assets/mazes/maze1.txt")};

    //Main player
    Character* character1{new Character(tilesTexture2, 16 * 1, 16 * 5, 16, 16, 
    SPRITE_SCALE, SPRITE_SCALE, new b2Vec2(400, 300), b2BodyType::b2_dynamicBody, world, window)};
    character1->SetAnimations(
        new Animation*[2]
        {
            new Animation(5, 0, 5, character1->GetSprite(), 40.f),
            new Animation(6, 0, 5, character1->GetSprite(), 80.f)
        }
    );
    character1->SetTagName("player");
    character1->SetPosition(400, 300);
    
    GameObject* chest{new GameObject(tilesTexture3, 16 * 19, 16 * 19, 16, 16, 
    SPRITE_SCALE, SPRITE_SCALE, new b2Vec2(400, 400), b2BodyType::b2_staticBody, world, window)}; 
    chest->SetTagName("item");

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

        //chestCollider->UpdatePhysics();

        if(sf::Joystick::isConnected(0))
        {
            character1->Move(new b2Vec2(joystickAxis->x * deltaTime * PLAYER_MOVESPEED, joystickAxis->y * deltaTime * PLAYER_MOVESPEED));
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
            character1->Move(new b2Vec2(keyboardAxis->x * deltaTime * PLAYER_MOVESPEED, keyboardAxis->y * deltaTime * PLAYER_MOVESPEED));
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

        window->clear(*(new sf::Color(150, 100, 0, 255)));//limpiar la pantalla
        
        for(auto& mazeTile : *maze1->GetContainer())
        {
            window->draw(*mazeTile->GetSprite());
        }

        //character1Collider->GetBoxShape()->setPosition(character1->GetSprite()->getPosition());

        //shinychest->Play(deltaTime);
        //window->draw(*chestCollider->GetBoxShape());

        lampAnimation->Play(deltaTime);
        window->draw(*firelamp);

        character1->Update();
        chest->Update();
        
        window->display(); //mostrar en pantalla lo que se va dibujar

        sf::Time timeElapsed = clock->getElapsedTime();
        deltaTime = timeElapsed.asMilliseconds();
        world->ClearForces();
        world->Step(1.f / 100 * deltaTime, 8, 8);
        clock->restart();

        //std::cout << "delta time: " << deltaTime << std::endl;

        delete keyboardAxis;
        delete joystickAxis;
    }
    
    return 0;
}