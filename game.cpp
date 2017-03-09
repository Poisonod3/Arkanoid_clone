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

  points = 0.0f;
  addedPoints = false;
  readLevels("levels.txt");

  timer = 0;
  slowMotion = false;
  slowMotionFactor = 0.25f;

  iCurrentLevel = 0;
  GameStarted = false;
  pause = false;
  gameOver = false;
  std::cout << "New game ready" << std::endl;
}

Game::~Game(){
  delete(pBall);
  delete(pPaddle);
  vecLevels.clear();
}

void Game::Update(sf::Event event){
  dTime = clock.restart();
  if(GameStarted)
  {
    timer += dTime.asSeconds();
  }

  if(slowMotion){
    dTime *= slowMotionFactor;
  }
  //std::cout << dTime.asSeconds() << std::endl;
  this->HandleInput(event);

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

    txt = "Points: " + std::to_string(points);
    addedPoints = false;
    this->CheckCollisions();
  }
}

void Game::NewGame(){
  delete(pBall);
  if(pPaddle->OutOfLives()){
    iCurrentLevel = 0;
    vecLevels.clear();
    readLevels("levels.txt");
    ResetPoints();
    delete(pPaddle);
    pPaddle = new Paddle();
  }
  textAnimationTimer = 0.0f;
  timer = 0;
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
  pPaddle->Dead();
  gameOver = true;
  GameStarted = false;
  std::cout << "GAME OVER!" << std::endl;
  std::cout << "You got " << points << " points\n";
}

void Game::CheckCollisions(){

  // ball can hit only once in update cycle
  bool collision = false;
  if(pBall->CheckCollision(bottomWall, false)){
    std::cout << "Ball out of bounds!\n";
    GameOver();
  }
  if(pBall->CheckCollision(topWall, false));
  if(pBall->CheckCollision(leftWall, false));
  if(pBall->CheckCollision(rightWall, false));

  //  ball hits paddle
  if(pBall->CheckCollision(pPaddle->GetRect(), true));

  // ball hits tile
  std::vector<Tile*>* vectiles = vecLevels[iCurrentLevel]->getTiles();
  std::vector<Tile*>::iterator it;
  for(it = vectiles->begin(); it < vectiles->end(); it++){
    if((*it)->IsActive() && !collision){
      if(pBall->CheckCollision((*it)->GetRect(), false)){
        // multiply points with speed of ball
        sf::Vector2f ballVelocity = pBall->getVelocity();
        float ballSpeed = fabs(ballVelocity.x) + fabs(ballVelocity.y);
        AddPoints(100.0f * log10(ballSpeed));
        addedPoints = true;

        // Yield
        Yield* pYield = new Yield;
        vecYield.push_back(pYield);

        pBall->SpeedUp(1.02f);
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
void Game::HandleInput(sf::Event event){
  // Input, Left and Right
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
  {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
      pPaddle->Move ( -1 );
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
      pPaddle->Move ( 1 );
    }
  }
  else{
    pPaddle->Move ( 0 );
  }

  if(!gameOver && !GameStarted && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
    StartGame();
  }

  if(gameOver && !GameStarted && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
    NewGame();
  }
  if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
  {
    this->SlowMotion();
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

void Game::AddPoints(float amount){
  this->points += amount;
}

void Game::ResetPoints(){
  this->points = 0.0f;
}

void Game::SlowMotion(){
  if(slowMotion)
  {
    slowMotion = false;
    std::cout << "Normal speed\n";
  }
  else
  {
    slowMotion = true;
    std::cout << "SlowMotion\n";
  }
}

void Game::Render(sf::RenderWindow* pWindow, sf::Font font){

  sf::Text textPoints(txt, font);
  textPoints.setCharacterSize(40);
  textPoints.setPosition(sf::Vector2f(25.0f, 25.0f));
  if(addedPoints || (!gameOver && textAnimationTimer != 0.0f))
  {
    if(addedPoints){
      textAnimationTimer = 0.0f;
    }
    textAnimationTimer += dTime.asSeconds();
    if(fmod(textAnimationTimer, 0.075f) <= 0.035f)
    {
      textPoints.setCharacterSize(41);
      textPoints.setColor(sf::Color::Blue);
    }
    else
    {
      textPoints.setColor(sf::Color::White);
    }

    if(textAnimationTimer >= 0.25f){
      textAnimationTimer = 0.0f;
      addedPoints = false;
    }
}else
  {
    textPoints.setCharacterSize(40);
    textPoints.setColor(sf::Color::White);
  }

  if(gameOver){
    textAnimationTimer += dTime.asSeconds();

    if(pPaddle->OutOfLives()){
      textPoints.setPosition(200, 200);
    }
    if(fmod(textAnimationTimer, 1.0f) < 0.5f)
    {
      textPoints.setCharacterSize(41);
      textPoints.setColor(sf::Color::Red);
    }
    else
    {
      textPoints.setColor(sf::Color::White);
    }
  }

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

  //  Draw player lives in top left corner of the screen
  if(!pPaddle->OutOfLives())
  {
    int playerLives = pPaddle->getLives();
    for(int i = 0; i < playerLives; i++){
      sf::RectangleShape shape;
      sf::RectangleShape paddleShape = pPaddle->GetRect();
      shape.setPosition(sf::Vector2f(25.0f + i * (paddleShape.getSize().x / 4 + 10.0f),
        paddleShape.getSize().y / 4));
      shape.setSize(sf::Vector2f(paddleShape.getSize().x / 4, paddleShape.getSize().y / 4));
      shape.setFillColor(sf::Color::Cyan);
      pWindow->draw(shape);
    }
  }

  pWindow->draw(textPoints);

  pWindow->display();
}
