#include <SFML/Graphics.hpp>

class Tile
{
protected:
  //sf::Vector2f position;
  //sf::Vector2f RectSize;
  bool active;
  int hits;
public:
  sf::RectangleShape shape;

  Tile();
  Tile(int h);
  Tile(sf::Vector2f newPos, int h);
  void Update();
  void Hit();
  void Destroy();
  bool IsActive();
  sf::RectangleShape GetRect();
  //sf::Vector2f GetPos();
  //sf::Vector2f GetRectSize();
};
