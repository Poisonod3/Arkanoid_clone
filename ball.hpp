#include <SFML/Graphics.hpp>
#include <string>
#include "gameobject.hpp"

class Ball: public GameObject
{
protected:
  bool released;
  float radius;

public:
  sf::CircleShape shape;

  Ball();
  ~Ball();
  void Start();
  void Update(sf::Time dTime);
  sf::Vector2f getPos();
  bool CheckCollision(sf::RectangleShape Rect, bool paddle);
  sf::Vector2f CalculateCollisionPoint(sf::Vector2f a);
  sf::Vector2f setNewDirection(sf::Vector2f a, sf::Vector2f b, bool paddle);
  float Distance(sf::Vector2f a, sf::Vector2f b);
  float ShortestDistance(sf::Vector2f a, sf::Vector2f b);
  void SpeedUp(float amount);
};
