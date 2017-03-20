#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "yield.hpp"

Yield::Yield(sf::Vector2f startPos){
  this->position = startPos;
  this->rotationSpeed = 1.0f;
  this->velocity = sf::Vector2f(0.0f, 100.0f);
  this->shape.setFillColor(sf::Color::White);
  this->shape.setSize(sf::Vector2f(25.0f, 25.0f));
  this->shape.setOrigin(sf::Vector2f(shape.getPosition() + sf::Vector2f(shape.getSize().x / 2, shape.getSize().y / 2)));
  this->active = true;

  std::cout << "Dropped yield!\n";
}

void Yield::Update(sf::Time dTime) {
  lastPos = position;
  setPosition(position + velocity * dTime.asSeconds());
  shape.setPosition(position);
  shape.rotate(rotationSpeed);
}

sf::RectangleShape Yield::GetRect(){
  return shape;
}

bool Yield::CheckCollision(sf::RectangleShape a) {
  if(shape.getGlobalBounds().intersects(a.getGlobalBounds())) {
    std::cout << "Yield collected\n";
    return true;
  }

  return false;
}

void Yield::Hit() {
  active = false;
}

bool Yield::isActive(){
  return active;
}
