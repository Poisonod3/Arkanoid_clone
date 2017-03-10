#include <SFML/Graphics.hpp>
#include "yield.hpp"
#include <math.h>

Yield::Yield(){
  this->position = sf::Vector2f(100.0f, 100.0f);
  this->velocity = sf::Vector2f(0.0f, -50.0f);
  this->shape.setFillColor(sf::Color::White);
  this->shape.setSize(sf::Vector2f(25.0f, 25.0f));

}

Yield::~Yield(){

}

sf::RectangleShape Yield::GetRect(){
  return shape;
}
