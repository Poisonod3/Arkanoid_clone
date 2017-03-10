#include <SFML/Graphics.hpp>
#include "gameobject.hpp"

GameObject::GameObject(){
  position.x = 0;
  position.y = 0;

  velocity.x = 0;
  velocity.y = 0;
}

GameObject::GameObject(sf::Vector2f newPosition){
  position.x = newPosition.x;
  position.y = newPosition.y;

  velocity.x = 0;
  velocity.y = 0;
}

GameObject::GameObject(sf::Vector2f newPosition, sf::Vector2f newVelocity){
  position.x = newPosition.x;
  position.y = newPosition.y;

  velocity.x = newVelocity.x;
  velocity.y = newVelocity.y;
}

GameObject::~GameObject(){

}

sf::Vector2f GameObject::getPosition(){
  return position;
}

sf::Vector2f GameObject::getVelocity(){
  return velocity;
}

void GameObject::setPosition(sf::Vector2f newPosition){
  position = newPosition;
}

void GameObject::setVelocity(sf::Vector2f newVelocity){
  velocity = newVelocity;
}
