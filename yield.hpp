#include <SFML/Graphics.hpp>
#include "gameobject.hpp"

class Yield: public GameObject
{
protected:
  sf::RectangleShape shape;
public:
  Yield();
  Yield(sf::Vector2f startPos);
  ~Yield();
  void Update(sf::Time dTime);
  sf::RectangleShape GetRect();
};
