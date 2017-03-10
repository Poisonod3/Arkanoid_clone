#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "yield.hpp"

Yield::Yield(){
  this->position = sf::Vector2f(100.0f, 100.0f);
  this->velocity = sf::Vector2f(0.0f, 100.0f);
  this->shape.setFillColor(sf::Color::White);
  this->shape.setSize(sf::Vector2f(25.0f, 25.0f));

  std::cout << "Dropped yield!\n";
}

Yield::Yield(sf::Vector2f startPos){
  this->position = startPos;
  this->velocity = sf::Vector2f(0.0f, 100.0f);
  this->shape.setFillColor(sf::Color::White);
  this->shape.setSize(sf::Vector2f(25.0f, 25.0f));

  std::cout << "Dropped yield!\n";
}

Yield::~Yield(){

}

void Yield::Update(sf::Time dTime) {
  setPosition(position + velocity * dTime.asSeconds());
  shape.setPosition(position);
}

sf::RectangleShape Yield::GetRect(){
  return shape;
}
