#include <SFML/Graphics.hpp>
#include "gameobject.hpp"

class Yield: public GameObject
{
protected:
  sf::RectangleShape shape;
public:
  Yield();
  ~Yield();
  sf::RectangleShape GetRect();
};
