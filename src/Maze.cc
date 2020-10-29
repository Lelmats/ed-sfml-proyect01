#include "Maze.hh"

Maze::Maze(unsigned int N, unsigned int M, float scale, float cropSize, sf::Texture*& texture, const char* path)
{
    this->N = N;
    this->M = M;
    this->scale = scale;
    this->cropSize = cropSize;
    this->texture = texture;
    this->path = path;
    tiles = new char*[N];
    reader->open(path);
    AllocateMemory();
    FillArray();
    Generate();
}

void Maze::AllocateMemory()
{
    for(int i{}; i < N; i++)
    {
        tiles[i] = new char[M];
    }
}

void Maze::FillArray()
{
    for(int i{}; i < N; i++)
    {
        for(int j{}; j < M; j++)
        {
            *reader >> tiles[i][j];
        }
    }
}

void Maze::Generate()
{
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {

            char& tile{tiles[i][j]};

            switch (tile)
            {
                case 'w':
                    container->push_back(new Tile(cropSize * 1, cropSize * 1, scale, cropSize, texture));
                    break;
                case 'e':
                    container->push_back(new Tile(cropSize * 1, cropSize * 2, scale, cropSize, texture));
                    break;
                case 'r':
                    container->push_back(new Tile(cropSize * 1, cropSize * 3, scale, cropSize, texture));
                    break;
                case 'g':
                    container->push_back(new Tile(cropSize * 1, cropSize * 4, scale, cropSize, texture));
                    break;
                case 'a':
                    container->push_back(new Tile(cropSize * 2, cropSize * 4, scale, cropSize, texture));
                    break;
                case 's':
                    container->push_back(new Tile(cropSize * 3, cropSize * 4, scale, cropSize, texture));
                    break;    
                case 'd':
                    container->push_back(new Tile(cropSize * 4, cropSize * 5, scale, cropSize, texture));
                    break;
                case 'i':
                    container->push_back(new Tile(cropSize * 4, cropSize * 3, scale, cropSize, texture));
                    break;
                case 'k':
                    container->push_back(new Tile(cropSize * 4, cropSize * 4, scale, cropSize, texture));
                    break;
                case 'u':
                    container->push_back(new Tile(cropSize * 3, cropSize * 6, scale, cropSize, texture));
                    break;
                case 'c':
                    container->push_back(new Tile(cropSize * 2, cropSize * 6, scale, cropSize, texture));
                    break;
                case 'v':
                    container->push_back(new Tile(cropSize * 3, cropSize * 6, scale, cropSize, texture));
                    break;              
                default:
                    break;
            }
            container->back()->Move(cropSize * scale * j, cropSize * scale * i);
        }
    }
}

std::vector<Tile*>* Maze::GetContainer() const
{
    return container;
}

char** Maze::GetTiles() const
{
    return tiles;
}

Maze::~Maze()
{
}