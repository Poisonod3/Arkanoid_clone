#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include "ball.hpp"

Ball::Ball () {
  position.x = 300;
  position.y = 450;
  radius = 20.0f;

  // setVelocity(sf::Vector2f(200.0f, -200.0f));

  shape.setRadius(radius);
  shape.setFillColor(sf::Color::White);
  shape.setPosition(sf::Vector2f(position.x - radius, position.y - radius));
  std::cout << "New ball created" << std::endl;
}

Ball::~Ball() {

}

void Ball::Start(){
  sf::Vector2f StartVector = sf::Vector2f((rand() % 8 - 4) * 10,
                                            ((rand() % 10 + 1) * 10) * -1);

  float length = sqrt((StartVector.x * StartVector.x) + (StartVector.y * StartVector.y));
  // normalize the vector
  StartVector.x /= length;
  StartVector.y /= length;
  StartVector *= 400.0f;
  setVelocity(StartVector);
}

void Ball::Update (sf::Time dTime) {
  lastPos = position;
  setPosition(position + velocity * dTime.asSeconds());
  shape.setPosition(sf::Vector2f(position.x - radius, position.y - radius));
}

sf::Vector2f Ball::getPos(){
  return position;
}

// function gets two arguments rectangleshape (!!next time use proper type!!)
// and bool if object is paddle (can't bounce downward from paddle)
bool Ball::CheckCollision(sf::RectangleShape Rect, bool paddle)
{
  //  Checks if collision has happened
  bool collision = false;
  //  Separate compoenents from rectangle
  sf::Vector2f RectPosition = Rect.getPosition(), RectSize = Rect.getSize();

  //  Getting corners from RectangleShape
  //  In order:  TopLeft, TopRight, BottomLeft, BottomRight;
  sf::Vector2f RectCorner[3];

  //  Pretty obvious
  RectCorner[0] = Rect.getPosition(); //  TopLeft
  RectCorner[1] = sf::Vector2f(RectPosition.x + RectSize.x, RectPosition.y); // TopRight
  RectCorner[2] = sf::Vector2f(RectPosition.x, RectPosition.y + RectSize.y); // BottomLeft
  RectCorner[3] = sf::Vector2f(RectPosition.x + RectSize.x, RectPosition.y + RectSize.y); // BottomRight

  //  Next few lines are almost straight from the interwebs
  float DeltaX = position.x - std::max(RectCorner[0].x, std::min(position.x, RectCorner[1].x));
  float DeltaY = position.y - std::max(RectCorner[0].y, std::min(position.y, RectCorner[2].y));

  //  But it works like charm..
  //  Checks if circle is inside the rectangle
  if((DeltaX * DeltaX + DeltaY * DeltaY) < (radius * radius)){
    //  if IS
    collision = true;

    //  if circle is on top of or under the rectangle on moment of collision
    //  no need to be go any further, just change direction and move on to next thing
    if(position.x > RectCorner[0].x && position.x < RectCorner[1].x){
      if(velocity.y < 0 && position.y > RectCorner[0].y && !paddle){
        // ball doesn't bounce downward from paddle
        velocity.y *= -1;
      }
      else if(velocity.y > 0 && position.y < RectCorner[2].y){
        velocity.y *= -1;
      }
      return collision;
    }

    //  same thing for the sides
    if(position.y > RectCorner[0].y && position.y < RectCorner[2].y){
        velocity.x *= -1;
        return collision;
    }

    //  But..
    //  if collision comes from another angle, we need to go a bit deeper

    //  Starting with TopLeft corner, we check every corner
    //  witch is closest to the ball
    sf::Vector2f NearestCorner = RectCorner[0];
    for(int i = 1; i < 4; i++){
      //  Measure the distance to previously nearest corner
      float distanceToNearest = Distance(position, NearestCorner);
      //  --||-- current corner
      float distanceToCurrent = Distance(position, RectCorner[i]);

      // Change current corner to nearest corner if thats true
      if(distanceToCurrent < distanceToNearest){
        NearestCorner = RectCorner[i];
      }
    }

    //  Now that we know witch corner is closest to the ball
    //  we can make an assumption where the ball was in the moment of impact
    sf::Vector2f collisionPoint = CalculateCollisionPoint(NearestCorner);
    //  Now we have the corner and the point of the collision
    //  Lets set the ball to the right direction
    setNewDirection(collisionPoint, NearestCorner, paddle);

  }

  return collision;
}

//  For this we only need the one corner which is closest to the ball
sf::Vector2f Ball::CalculateCollisionPoint(sf::Vector2f a){
  //  We are making an assumption where the ball was on impact
  //  We can make close enough assumption with just five points
  sf::Vector2f ballPosition[4];
  //  We need to know where the ball WAS going
  sf::Vector2f ballDirection = sf::Vector2f(lastPos - position);
  float length = sqrt((ballDirection.x * ballDirection.x) + (ballDirection.y * ballDirection.y));
  //  normalize the vector
  ballDirection.x /= length;
  ballDirection.y /= length;

  //  Start the checking from last known position
  ballPosition[0] = lastPos;
  //  Next position will be quater of the way to the current position from last position
  ballPosition[1] = sf::Vector2f(lastPos + ballDirection * (length / 4.0f));
  //  --||-- half --||--
  ballPosition[2] = sf::Vector2f(lastPos + ballDirection * (length / 2.0f));
  //  --||-- three quaters --||--
  ballPosition[3] = sf::Vector2f(position + ballDirection * (-length / 4.0f));
  //  Usually not getting this far
  ballPosition[4] = position;

  //  Then to calculate which was closest
  //  Starting with the last position, going down to current position
  sf::Vector2f NearestPosition = ballPosition[0];
  for(int i = 1; i < 5; i++){
    float shortestDistance = ShortestDistance(NearestPosition, a);
    float currentDistance = ShortestDistance(ballPosition[i], a);

    if(currentDistance < shortestDistance){
      NearestPosition = ballPosition[i];
    }
  }
  return NearestPosition;
}

sf::Vector2f Ball::setNewDirection(sf::Vector2f a, sf::Vector2f b, bool paddle){
    // make relative vector from corner position to ball position
    sf::Vector2f rVector = sf::Vector2f(a - b);
    // get length of the vector
    float length = sqrt((rVector.x * rVector.x) + (rVector.y * rVector.y));
    // normalize the vector
    rVector.x /= length;
    rVector.y /= length;
    if(paddle && rVector.y < 0){
      rVector.y *= -1;
    }
    // get vectors angle (in degrees)
    float angle = atan2(rVector.y, rVector.x)* 180 / 3.145f;
    //std::cout << angle << "\n";

    // get vectors lenght
    length = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));

    // set new angle and multiply with lenght
    velocity.x = sin(angle);
    velocity.y = cos(angle);
    velocity *= length;

    return rVector;
}

float Ball::Distance(sf::Vector2f a, sf::Vector2f b){
  float distance = fabs(a.x - b.x) + fabs(a.y - b.y);
  return distance;
}

float Ball::ShortestDistance(sf::Vector2f a, sf::Vector2f b){
  float d = std::min(fabs(radius / 2 - Distance(a, b)),
    std::min(fabs(radius / 2 - Distance(a, sf::Vector2f(a.x, b.y))),
    fabs(radius / 2 - Distance(a, sf::Vector2f(a.y, b.x)))));

  return d;
}

void Ball::SpeedUp(float amount){
  this->velocity *= amount;
}
