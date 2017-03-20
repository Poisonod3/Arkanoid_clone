#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "paddle.hpp"

Paddle::Paddle () {
  this->shape.setPosition(sf::Vector2f(300.0f, 500.0f));
  this->shape.setSize(sf::Vector2f(100.0f, 20.0f));
  this->shape.setFillColor(sf::Color::Cyan);
  this->speed =         sf::Vector2f(0.0f, 0.0f);
  this->targetSpeed =   sf::Vector2f(0.0f, 0.0f);
  this->acceleration =  sf::Vector2f(100.0f, 1.0f);
  this->MAX_speed =     sf::Vector2f(20.0f, 1.0f);
  this->targetPosition = sf::Vector2f(0.0f, 0.0f);
  this->lives = 3;

  std::cout << "New Paddle Created" << std::endl;
}

Paddle::Paddle (int newLives) {
  this->shape.setPosition(sf::Vector2f(300.0f, 500.0f));
  this->shape.setSize(sf::Vector2f(100.0f, 20.0f));
  this->shape.setFillColor(sf::Color::Cyan);
  this->speed =         sf::Vector2f(0.0f, 0.0f);
  this->targetSpeed =   sf::Vector2f(0.0f, 0.0f);
  this->acceleration =  sf::Vector2f(100.0f, 1.0f);
  this->MAX_speed =     sf::Vector2f(20.0f, 1.0f);
  this->targetPosition = sf::Vector2f(0.0f, 0.0f);
  this->lives = newLives;

  std::cout << "New Paddle Created" << std::endl;
}

void Paddle::Update(sf::Time dTime)
{
  // Accelerate to target speed
  if(this->speed.x != this->targetSpeed.x)
  {
    if( this->speed.x < this->targetSpeed.x ) {
      this->speed.x += this->acceleration.x * dTime.asSeconds();
    }

    if( this->speed.x > this->targetSpeed.x ) {
      this->speed.x -= this->acceleration.x * dTime.asSeconds();
    }
    // This prevents sliding
    if(targetSpeed.x == 0.0f && fabs(speed.x - 0.001f) <= (acceleration.x * dTime.asSeconds())){
      //std::cout << fabs(speed.x) << std::endl;
      speed.x = 0.0f;
    }
  }

  shape.setPosition(this->shape.getPosition() + this->speed);
  this->CheckCollision();
}

void Paddle::Move(int direction){
  if (direction != 0){
    targetSpeed.x = MAX_speed.x * direction;
    //this->speed.x = this->MAX_speed.x * direction;
  }
  else {
    targetSpeed.x = 0;
  }
}

void Paddle::CheckCollision(){
  sf::Vector2f position = shape.getPosition();
  sf::Vector2f size = shape.getSize();

  //hits wall
  if(position.x <= 0.0f){
    speed.x = 0.0f;
    shape.setPosition(sf::Vector2f(0.0f, position.y));
  }

  if(position.x + size.x >= 600.0f){
    speed.y = 0.0f;
    shape.setPosition(sf::Vector2f(600.0f - size.x, position.y));
  }
}
sf::RectangleShape Paddle::GetRect(){
  return shape;
}

sf::Vector2f Paddle::GetPos(){
  return shape.getPosition();
}

sf::Vector2f Paddle::GetRectSize(){
  return shape.getSize();
}

void Paddle::SetTargetPosition(sf::Vector2f newTargetPos){
  this->targetPosition = newTargetPos;
}

int Paddle::getLives(){
  return lives;
}

bool Paddle::OutOfLives(){
  if(lives == 0)
    return true;
  else
    return false;
}

void Paddle::Dead(){
  lives--;
}
