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

  void HandleInput();
  void CheckCollisions();
  int iLevel;
  int iCurrentLevel;
  std::vector<Level*> vecLevels;
  std::vector<Tile*> vecCurrTiles;
  bool GameStarted;
  bool gameOver;
  sf::Clock clock;
  float timer;
public:

  Game();

  void NewGame();
  void StartGame();
  void NextLevel();
  void GameOver();
  void Update(sf::Event event);
  void Render(sf::RenderWindow* pWindow);
  void readLevels(std::string filename);
};
