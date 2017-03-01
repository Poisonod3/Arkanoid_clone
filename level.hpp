#include <SFML/Graphics.hpp>
#include <vector>

#include "tile.hpp"

class Level
{
private:
  int LevelId;
  std::vector<Tile*>* vecTiles;

public:
  Level(int id);
  std::vector<Tile*>* getTiles(){
    return vecTiles;
  };

  void addTile(Tile* tile);
};
