#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

#include "paddle.hpp"
#include "ball.hpp"
#include "level.hpp"
#include "yield.hpp"

struct Highscore {
  std::string nickname;
  int score;
};

class Game{
private:
  Ball* pBall;
  Paddle* pPaddle;

  sf::SoundBuffer buffer;
  sf::Sound BoingSound;

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
  bool pause;
  int points;
  std::vector<Highscore> Highscores;
  float multiplier;
  float multiplierTimer;

  sf::Clock clock;
  sf::Time dTime;
  float timer;
  bool slowMotion;
  float slowMotionFactor;
  std::string txt;
  bool addedPoints;
  float textAnimationTimer;
public:

  Game();
  ~Game();

  void NewGame();
  void StartGame();
  void StartGame(sf::Vector2f mousePos);
  void Pause();
  void NextLevel();
  void GameOver();
  void Update(sf::Event event);
  void Render(sf::RenderWindow* pWindow, sf::Font font);
  void readLevels(std::string filename);
  void AddPoints(float amount);
  void AddMultiplier();
  void AddMultiplier(int k);
  void ResetPoints();
  void ReadHighscores(std::string filename);
  void PrintHighscores();
  void SortHighscores();
  bool CheckIfHighscore();
  void SetNewHighscores();
  void SlowMotion();
};
