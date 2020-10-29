#pragma once
#include<SFML/Graphics.hpp>

class Tile
{
    private:
        sf::Sprite* sprite;
    public:
        Tile(float, float, float, float, sf::Texture*&);
        ~Tile();
        void Move(float, float);
        void SetPosition(float, float);
        sf::Sprite* GetSprite() const;
};