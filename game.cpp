#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "game.hpp"

Game::Game(){
  pBall = new Ball();
  pPaddle = new Paddle();

  bottomWall.setPosition(sf::Vector2f(-50.0f, 640.0f)); // bottombottomWall, instadeath
  bottomWall.setSize(sf::Vector2f(700.0f, 50.0f));

  topWall.setPosition(sf::Vector2f(-50.0f, -50.0f)); // toptopWall
  topWall.setSize(sf::Vector2f(700.0f, 50.0f));

  leftWall.setPosition(sf::Vector2f(-50.0f, -50.0f)); // leftleftWall
  leftWall.setSize(sf::Vector2f(50.0f, 700.0f));

  rightWall.setPosition(sf::Vector2f(600.0f, -50.0f)); //
  rightWall.setSize(sf::Vector2f(50.0f, 700.0f));

  readLevels("levels.txt");

  timer = 0;

  iCurrentLevel = 0;
  GameStarted = false;
  gameOver = false;
  std::cout << "New game ready" << std::endl;
}

void Game::Update(sf::Event event){
  sf::Time dTime = clock.restart();

  if(GameStarted)
  {
    timer += dTime.asSeconds();
  }
  //std::cout << dTime.asSeconds() << std::endl;
  this->HandleInput();

  if(!gameOver){
    pPaddle->Update(dTime);
    pBall->Update(dTime);
    std::vector<Tile*>* vectiles = vecLevels[iCurrentLevel]->getTiles();
    std::vector<Tile*>::iterator it;
    int activeTiles = 0;
    for(it = vectiles->begin(); it < vectiles->end(); it++){
      if((*it)->IsActive()){
        (*it)->Update();
        activeTiles++;
      }
    }
    if(activeTiles == 0){
      NextLevel();
    }

    this->CheckCollisions();
  }
}

void Game::NewGame(){
  delete(pBall);
  //vecLevels.clear();
  //readLevels("levels.txt");
  timer = 0;
  iCurrentLevel = 0;
  GameStarted = false;
  gameOver = false;
  pBall = new Ball();

  std::cout << "New game ready" << std::endl;
}

void Game::StartGame(){
  gameOver = false;
  GameStarted = true;
  pBall->Start();
  std::cout << "Game started" << std::endl;
}

void Game::NextLevel(){
  delete(pBall);
  iCurrentLevel++;
  GameStarted = false;
  pBall = new Ball();
  timer = 0;

  std::cout << "Next level ready\n";
}

void Game::GameOver(){
  gameOver = true;
  GameStarted = false;
  std::cout << "GAME OVER!" << std::endl;
}

void Game::CheckCollisions(){

  if(pBall->CheckCollision(bottomWall, false)){
    std::cout << "Ball out of bounds!\n";
    GameOver();
  }
  if(pBall->CheckCollision(topWall, false));
  if(pBall->CheckCollision(leftWall, false));
  if(pBall->CheckCollision(rightWall, false));

  //ball hits paddle
  if(pBall->CheckCollision(pPaddle->GetRect(), true));

  std::vector<Tile*>* vectiles = vecLevels[iCurrentLevel]->getTiles();
  std::vector<Tile*>::iterator it;
  for(it = vectiles->begin(); it < vectiles->end(); it++){
    if((*it)->IsActive()){
      if(pBall->CheckCollision((*it)->GetRect(), false)){
        (*it)->Hit();
      }
    }
  }

  /*
  if(pTile->IsActive()){
    if(pBall->CheckCollision(pTile->GetRect(), false)){
      pTile->Destroy();
    }
  }*/
}

//void Game::HandleInput(sf::Event event){
void Game::HandleInput(){
  // Input, Left and Right
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      pPaddle->Move ( -1 );
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      pPaddle->Move ( 1 );
    }
  }
  if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    pPaddle->Move ( 0 );
  }

  if(!gameOver && !GameStarted && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
    StartGame();
  }

  if(gameOver && !GameStarted && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
    NewGame();
  }
}

void Game::readLevels(std::string filename){
  std::cout << "Reading levels\n";
  std::ifstream ifs;
  ifs.open(filename.c_str());

  if(ifs.is_open()){
    std::string line;

    iLevel = 0;
    Level* pLevel = new Level(iLevel);
    Tile* pTile;
    int iTileCol = 0;
    int iTileRow = 0;
    int iTiles = 0;

    while(getline(ifs, line)){
      for(char& c : line){
        if(c == '-'){
          std::cout << "Added " << iTiles << " tiles" << std::endl;
          iLevel++;
          iTileRow = 0;
          iTileCol = 0;
          vecLevels.insert (vecLevels.end(),pLevel);
          pLevel = new Level(iLevel);
          iLevel++;
          break;
        }

        int hits = c - '0';
        if(hits > 0 && hits < 10){
          int x = 50 + iTileCol * 70;
          int y = 50 + iTileRow * 40;
          pTile = new Tile(sf::Vector2f(x, y), hits);
          pLevel->addTile(pTile);
          iTiles++;
        }
        iTileCol++;
      }
      iTileCol = 0;
      iTileRow++;
    }
    std::cout << "Created " << iLevel << " levels\n";
  }
}

void Game::Render(sf::RenderWindow* pWindow){
  pWindow->clear();

  std::vector<Tile*>* vectiles = vecLevels[iCurrentLevel]->getTiles();
  std::vector<Tile*>::iterator it;
  for(it = vectiles->begin(); it < vectiles->end(); it++){
    if((*it)->IsActive()){
      pWindow->draw((*it)->shape);
    }
  }

  pWindow->draw(pPaddle->GetRect());
  pWindow->draw(pBall->shape);
  pWindow->display();
}
