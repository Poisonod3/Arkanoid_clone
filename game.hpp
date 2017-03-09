#include <SFML/Graphics.hpp>
#include <vector>

#include "paddle.hpp"
#include "ball.hpp"
#include "level.hpp"
#include "yield.hpp"

class Game{
private:
  Ball* pBall;
  Paddle* pPaddle;

  sf::RectangleShape bottomWall;
  sf::RectangleShape topWall;
  sf::RectangleShape leftWall;
  sf::RectangleShape rightWall;

  void HandleInput(sf::Event event);
  void CheckCollisions();
  int iLevel;
  int iCurrentLevel;
  std::vector<Level*> vecLevels;

  std::vector<Yield*> vecYield;

  bool GameStarted;
  bool gameOver;
  int points;
  sf::Clock clock;
  sf::Time dTime;
  float timer;
  float slowMotionFactor;
  std::string txt;
  bool addedPoints;
  float textAnimationTimer;
public:

  Game();
  ~Game();
  bool pause;
  bool slowMotion;

  void NewGame();
  void StartGame();
  void NextLevel();
  void GameOver();
  void Update(sf::Event event);
  void Render(sf::RenderWindow* pWindow, sf::Font font);
  void readLevels(std::string filename);
  void AddPoints(float amount);
  void ResetPoints();
  void SlowMotion();
};
