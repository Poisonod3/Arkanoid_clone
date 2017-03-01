#include <SFML/Graphics.hpp>

class GameObject
{
protected:
  sf::Vector2f position, velocity, lastPos;

public:
  GameObject();
  GameObject(sf::Vector2f newPosition);
  GameObject(sf::Vector2f newPosition, sf::Vector2f newVelocity);
  ~GameObject();
  void Update(sf::Time dTime);
  sf::Vector2f getPosition();
  sf::Vector2f getVelocity();
  void setPosition(sf::Vector2f newPosition);
  void setVelocity(sf::Vector2f newVelocity);
};
