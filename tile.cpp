#include <SFML/Graphics.hpp>
#include <iostream>
#include "tile.hpp"

Tile::Tile () {
  shape.setPosition(sf::Vector2f(300.0f, 100.0f));
  shape.setSize(sf::Vector2f(50.0f, 20.0f));
  shape.setFillColor(sf::Color::Blue);
  active = true;

  //std::cout << "New tile created" << std::endl;
}

Tile::Tile (int h) {
  shape.setPosition(sf::Vector2f(300.0f, 100.0f));
  shape.setSize(sf::Vector2f(50.0f, 20.0f));
  shape.setFillColor(sf::Color::Blue);
  active = true;

  //std::cout << "New tile created" << std::endl;
}

Tile::Tile (sf::Vector2f newPos, int h) {
  shape.setPosition(newPos);
  shape.setSize(sf::Vector2f(50.0f, 20.0f));
  hits = h;
  active = true;

  //std::cout << "New tile created" << std::endl;
}

void Tile::Update()
{
  switch(hits) {
    case 1:
      shape.setFillColor(sf::Color::Blue);
      break;
    case 2:
      shape.setFillColor(sf::Color::Red);
      break;
    case 3:
      shape.setFillColor(sf::Color::Green);
      break;
    case 0:
      active = false;
      break;
    default:
      shape.setFillColor(sf::Color::White);
      break;
  }
}

int Tile::Hit(){
  hits--;
  return hits + 1;
}

void Tile::Destroy(){
  if(active){
    std::cout << "Tile destroyed!" << std::endl;
    active = false;
  }
}

sf::RectangleShape Tile::GetRect(){
  return shape;
}

bool Tile::IsActive(){
  return active;
}
