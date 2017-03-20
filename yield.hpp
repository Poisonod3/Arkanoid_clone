#include <SFML/Graphics.hpp>
#include "gameobject.hpp"

class Yield: public GameObject
{
protected:
  sf::RectangleShape shape;
  bool active;
  int type;
  float rotationSpeed;

public:
  Yield(sf::Vector2f startPos);
  void Update(sf::Time dTime);
  sf::RectangleShape GetRect();
  bool CheckCollision(sf::RectangleShape a);
  void Hit();
  bool isActive();
};
