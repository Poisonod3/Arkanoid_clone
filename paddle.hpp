#pragma once
#include <SFML/Graphics.hpp>

class Paddle
{
protected:
  sf::Vector2f speed, MAX_speed, acceleration, targetSpeed, targetPosition;
  sf::RectangleShape shape;
public:

  Paddle();
  void Update(sf::Time dTime);
  void Move(int direction);
  void CheckCollision();
  sf::RectangleShape GetRect();
  sf::Vector2f GetPos();
  sf::Vector2f GetRectSize();
  void SetTargetPosition(sf::Vector2f newTargetPos);
};
