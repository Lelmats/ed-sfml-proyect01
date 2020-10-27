#include<iostream>
#include <SFML/Graphics.hpp>
#include<box2d/box2d.h>

#include "Maze.hh"
#include "Inputs.hh"
#include "Character.hh"
#include "Tile.hh"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define GAME_NAME "Roguelike game"
#define TILES1 "assets/sprites/tiles1.png"
#define TILES2 "assets/sprites/tiles2.png"
#define TILES3 "assets/sprites/tiles3.png"
#define SPRITE_SCALE 4.f
#define FPS 144
#define PLAYER_MOVESPEED 2.5f

int main()
{
    //esto es la ventana de tu grafico
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);
    //aqui vas a guardar los eventos dentro de la ventana, eje: teclado, mouse, etc.
    sf::Event event;
  

    //physics declaration
    b2Vec2* gravity{new b2Vec2(0.f, 0.f)};
    b2World* world{new b2World(*gravity)}; 

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
    sf::Sprite* chest{new sf::Sprite(*tilesTexture2, *(new sf::IntRect(16 * 6, 16 * 1, 16, 16)))};
    chest->setScale(SPRITE_SCALE, SPRITE_SCALE);
    chest->setPosition(64 * 11,64 * 1.85);
    Animation* shinychest{new Animation(1,6,13,chest,130)};

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

    BoxCollider* chestCollider = new BoxCollider(64 * 0,50 * 2, new sf::Color(0, 255, 0, 255), 16, 16,
    new Rigidbody(world, b2BodyType::b2_staticBody, new b2Vec2(32 *22,32 * 3.3), 32, 32, 1, 0, 0),chest);
    
    chestCollider->GetBoxShape()->setScale(SPRITE_SCALE, SPRITE_SCALE);
    chestCollider->GetBoxShape()->setPosition(chest->getPosition());

    
/*
    char** lab
    {
        new char*[10]
        {
            new char[13]{'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'},
            new char[13]{'w', 'e', 'r', 'w', 'd', 'i', 'w', 'i', 'r', 'w', 'e', 'r', 'w'},
            new char[13]{'g', 'g', 'g', 's', 'g', 'k', 'u', 'k', 'g', 'g', 'g', 'g', 'g'},
            new char[13]{'g', 'g', 'g', 'g', 's', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g'},
            new char[13]{'s', 'g', 'a', 's', 'g', 'g', 'g', 'g', 'g', 's', 'g', 'g', 'g'},
            new char[13]{'g', 'g', 'g', 'g', 'g', 'g', 's', 'g', 'g', 'g', 'g', 'g', 'g'},
            new char[13]{'g', 'g', 'g', 'g', 'g', 'g', 'a', 'g', 's', 'g', 'g', 'g', 'g'},
            new char[13]{'g', 'g', 's', 'g', 'g', 'g', 'a', 'g', 'g', 'g', 'g', 'g', 'g'},
            new char[13]{'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'a', 'g', 'g', 'g'},
            new char[13]{'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g'}
        }
    };
*/
    /*std::ifstream* reader{new std::ifstream()};
    reader->open("assets/mazes/maze1.txt");

    unsigned int N{10}, M{13};

    char** tiles{new char*[N]};

    //reservando memoria, dentro del array
    for(int i{}; i < N; i++)
    {
        tiles[i] = new char[M];
    }

    for(int i{}; i < N; i++)
    {
        for(int j{}; j < M; j++)
        {
            *reader >> tiles[i][j];
        }
    }*/


/*
    for (int i ={}; i < 10; i++)
    {
        for (int j = 0; j < 13; j++)
        {
        //char& l = *(*(lab + i) + j);
        char& tile{tiles[j][i]};
           switch (tile)
           {
           case 'w':
                Spritemaze.push_back(*tileWall1);
                Spritemaze.back().setPosition(64 * j, 64 * i);
               break;
           case 'e':
                Spritemaze.push_back(*tileWall2);
                Spritemaze.back().setPosition(64 * j, 64 * i);
               break;
            case 'r':
                Spritemaze.push_back(*tileWall3);
                Spritemaze.back().setPosition(64 * j, 64 * i);
               break;
            case 'g':
                Spritemaze.push_back(*tileGround1);
                Spritemaze.back().setPosition(64 * j, 64 * i);
               break;
            case 'a':
                Spritemaze.push_back(*tileGround2);
                Spritemaze.back().setPosition(64 * j, 64 * i);
               break;
            case 's':
                Spritemaze.push_back(*tileGround3);
                Spritemaze.back().setPosition(64 * j, 64 * i);
               break;
            case 'd':
                Spritemaze.push_back(*tileWall4);
                Spritemaze.back().setPosition(64 * j, 64 * i);
               break;
            case 'i':
                Spritemaze.push_back(*tileWall1_1);
                Spritemaze.back().setPosition(64 * j, 64 * i);
            break;
            case 'k':
                Spritemaze.push_back(*tileWall1_2);
                Spritemaze.back().setPosition(64 * j, 64 * i);
               break;
            case 'u':
                Spritemaze.push_back(*stairs);
                Spritemaze.back().setPosition(64 * j, 64 * i);
               break;
           default:
               break;
           }
        }
    } */
//Maze
    unsigned int N{10}, M{13};
    Maze* maze1{new Maze(N, M, "assets/mazes/maze1.txt")};

    std::vector<Tile*>* maze{new std::vector<Tile*>()};

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {

            char& tile{maze1->GetTiles()[i][j]};

            switch (tile)
            {
                case 'w':
                    //maze.push_back(*tileWall_1_1);
                    maze->push_back(new Tile(16 * 1, 16 * 1, SPRITE_SCALE, 16, tilesTexture3));
                    break;
                case 'e':
                    maze->push_back(new Tile(16 * 1, 16 * 2, SPRITE_SCALE, 16, tilesTexture3));
                    break;
                case 'r':
                    maze->push_back(new Tile(16 * 1, 16 * 3, SPRITE_SCALE, 16, tilesTexture3));
                    break;
                case 'g':
                    maze->push_back(new Tile(16 * 1, 16 * 4, SPRITE_SCALE, 16, tilesTexture3));
                    break;
                case 'a':
                    maze->push_back(new Tile(16 * 2, 16 * 4, SPRITE_SCALE, 16, tilesTexture3));
                    break;
                case 's':
                    maze->push_back(new Tile(16 * 3, 16 * 4, SPRITE_SCALE, 16, tilesTexture3));
                    break;    
                case 'd':
                    maze->push_back(new Tile(16 * 4, 16 * 5, SPRITE_SCALE, 16, tilesTexture3));
                    break;
                case 'i':
                    maze->push_back(new Tile(16 * 4, 16 * 3, SPRITE_SCALE, 16, tilesTexture3));
                    break;
                case 'k':
                    maze->push_back(new Tile(16 * 4, 16 * 4, SPRITE_SCALE, 16, tilesTexture3));
                    break;
                case 'u':
                    maze->push_back(new Tile(16 * 3, 16 * 6, SPRITE_SCALE, 16, tilesTexture3));
                    break;
                case 'c':
                    maze->push_back(new Tile(16 * 2, 16 * 6, SPRITE_SCALE, 16, tilesTexture3));
                    break;
                case 'v':
                    maze->push_back(new Tile(16 * 3, 16 * 6, SPRITE_SCALE, 16, tilesTexture3));
                    break;              
                default:
                    break;
            }
            maze->back()->Move(tileBaseWidth * j, tileBaseHeight * i);
        }
    }

    //Main player
    Character* character1{new Character(tilesTexture2, 16 * 1, 16 * 5, 16, 16, SPRITE_SCALE, SPRITE_SCALE, world, window)};
    character1->SetAnimations(
    new Animation*[2]
        {
            new Animation(5, 0, 5, character1->GetSprite(), 40.f),
            new Animation(6, 0, 5, character1->GetSprite(), 80.f)
        }
    );

    character1->SetPosition(400, 300);

    /*BoxCollider* character1Collider = new BoxCollider(400, 300, new sf::Color(0, 255, 0, 255), 16, 16,
    new Rigidbody(world, b2BodyType::b2_dynamicBody, new b2Vec2(400, 300), 32, 32 , 1, 0, 0),
    character1->GetSprite());
    character1Collider->GetBoxShape()->setScale(SPRITE_SCALE, SPRITE_SCALE);*/

    //physics player
    b2BodyDef* playerBodyDef {new b2BodyDef()};
    playerBodyDef->type = b2BodyType::b2_dynamicBody;
    playerBodyDef->position = *(new b2Vec2(character1->GetSprite()->getPosition().x, character1->GetSprite()->getPosition().y));

    b2Body* playerBody = world->CreateBody(playerBodyDef);
    b2PolygonShape* playerPolygonShape{new b2PolygonShape()};
    playerPolygonShape->SetAsBox(32,32); //la X debe ser la mitad original y la Y también debe ser la mitad

    b2FixtureDef* playerFixtureDef{new b2FixtureDef()};
    playerFixtureDef->shape = playerPolygonShape;
    playerFixtureDef->density = 1;
    playerFixtureDef->friction = 0;
    playerFixtureDef->restitution = 0;

    b2Fixture* playerFixture{playerBody->CreateFixture(playerFixtureDef)};

    //treasure physics

    b2BodyDef* treasureBodyDef{new b2BodyDef()};
    treasureBodyDef->type = b2BodyType::b2_staticBody;
    treasureBodyDef->position = *(new b2Vec2(chest->getPosition().x, chest->getPosition().y));

    b2Body* treasureBody{world->CreateBody(treasureBodyDef)};
    b2PolygonShape* treasurePolygonShape{new b2PolygonShape()};
    treasurePolygonShape->SetAsBox(32,32); 

    b2FixtureDef* treasureFixtureDef{new b2FixtureDef()};
    treasureFixtureDef->shape = treasurePolygonShape;
    treasureFixtureDef->density = 1; 
    treasureFixtureDef->friction = 0; 
    treasureFixtureDef->restitution = 0; 

    b2Fixture* treasureFixture{treasureBody->CreateFixture(treasureFixtureDef)};
    
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

        //player sigue la posicion del cuerpo de física
        //character1Collider->UpdatePhysics();
        chestCollider->UpdatePhysics();

        //character1->GetSprite()->setPosition(character1Collider->GetBodyPosition().x, character1Collider->GetBodyPosition().y);
        //chest->setPosition(chestCollider->GetBodyPosition().x, chestCollider->GetBodyPosition().y);


        //character1->GetSprite()->setPosition(playerBody->GetPosition().x, playerBody->GetPosition().y);
        //chest->setPosition(treasureBody->GetPosition().x, treasureBody->GetPosition().y);

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
        
        for(auto& mazeTile : *maze)
        {
            window->draw(*mazeTile->GetSprite());
        }


        //character1Collider->GetBoxShape()->setPosition(character1->GetSprite()->getPosition());

        shinychest->Play(deltaTime);
        window->draw(*chest);
        window->draw(*chestCollider->GetBoxShape());

        lampAnimation->Play(deltaTime);
        window->draw(*firelamp);

        character1->Update();

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