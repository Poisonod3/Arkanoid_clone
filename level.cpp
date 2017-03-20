#include "level.hpp"

Level::Level(int id){
    LevelId = id;
    vecTiles = new std::vector<Tile*>();
}

void Level::addTile(Tile* tile)
{
  vecTiles->push_back(tile);
}
