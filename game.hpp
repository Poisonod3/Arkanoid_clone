#include <SFML/Graphics.hpp>
#include <vector>

#include "paddle.hpp"
#include "ball.hpp"
#include "level.hpp"

class Game{
private:
  Ball* pBall;
  Paddle* pPaddle;
  Tile* pTile;

  sf::RectangleShape bottomWall;
  sf::RectangleShape topWall;
  sf::RectangleShape leftWall;
  sf::RectangleShape rightWall;

  void HandleInput(sf::Event event);
  void CheckCollisions();
  int iLevel;
  int iCurrentLevel;
  std::vector<Level*> vecLevels;
  std::vector<Tile*> vecCurrTiles;
  bool GameStarted;
  bool gameOver;
  int points;
  sf::Clock clock;
  float timer;
  float slowMotionFactor;
public:

  Game();
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
