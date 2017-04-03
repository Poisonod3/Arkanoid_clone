#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "game.hpp"

Game::Game(){
  pBall = new Ball();
  pPaddle = new Paddle();

  if(!buffer.loadFromFile("./sound/boing.wav"));


  bottomWall.setPosition(sf::Vector2f(-50.0f, 640.0f)); // bottombottomWall, instadeath
  bottomWall.setSize(sf::Vector2f(700.0f, 50.0f));

  topWall.setPosition(sf::Vector2f(-50.0f, -50.0f)); // toptopWall
  topWall.setSize(sf::Vector2f(700.0f, 50.0f));

  leftWall.setPosition(sf::Vector2f(-50.0f, -50.0f)); // leftleftWall
  leftWall.setSize(sf::Vector2f(50.0f, 700.0f));

  rightWall.setPosition(sf::Vector2f(600.0f, -50.0f)); //
  rightWall.setSize(sf::Vector2f(50.0f, 700.0f));

  points = 0;
  multiplier = 1.0f;
  addedPoints = false;
  readLevels("levels.txt");

  timer = 0;
  slowMotion = false;
  slowMotionFactor = 0.25f;

  iCurrentLevel = 0;
  InputNameState = false;
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

  if (!InputNameState){
    HandleInput(event);

    if(multiplier > 1.0f) {
      multiplierTimer += dTime.asSeconds();
      if(multiplierTimer > 2.0f) {
        multiplier--;
        multiplierTimer = 0.0f;
      }
    }
    if(multiplier < 1.0f) {
      multiplier = 1.0f;
    }

    if(!gameOver && !pause){
      pPaddle->Update(dTime);
      pBall->Update(dTime);

      for(std::vector<Yield*>::iterator it = vecYield.begin(); it < vecYield.end(); it++){
        (*it)->Update(dTime);
      }

      std::vector<Tile*>* vectiles = vecLevels[iCurrentLevel]->getTiles();
      std::vector<Tile*>::iterator it;
      int activeTiles = 0;

      if(vectiles != NULL){
        for(it = vectiles->begin(); it < vectiles->end(); it++){
          if((*it)->IsActive()){
            (*it)->Update();
            activeTiles++;
          }
        }
      }
      if(activeTiles == 0){
        NextLevel();
      }

      txt = "Points: " + std::to_string(points);
      if(multiplier > 1.0f){
        txt.append(" x");
        txt.append(std::to_string(int(multiplier)));
      }
      addedPoints = false;
      this->CheckCollisions();
    }
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

void Game::StartGame(sf::Vector2f mousePos){
  gameOver = false;
  GameStarted = true;
  pBall->Start(mousePos);
  std::cout << "Game started" << std::endl;
}

void Game::Pause () {
  pause = !pause;
}

void Game::NextLevel(){
  delete(pBall);
  vecYield.clear();
  iCurrentLevel++;
  GameStarted = false;
  pBall = new Ball();
  timer = 0;

  std::cout << "Next level ready\n";
}

void Game::GameOver(){
  pPaddle->Dead();
  vecYield.clear();
  multiplier = 1.0f;
  gameOver = true;
  GameStarted = false;
  if(pPaddle->OutOfLives()){
    std::cout << "GAME OVER!" << std::endl;
    std::cout << "You got " << points << " points\n";
    ReadHighscores("highscores.txt");
    if(CheckIfHighscore()){
      std::cout << "New Highscore!\n";
      InputNameState = true;
    }
  }
}

bool Game::InputState () {
  return InputNameState;
}

void Game::InputName (std::string name) {
  std::cout << "Inputting name\n";
  SetNewHighscores(name, "highscores.txt");
  InputNameState = false;
  PrintHighscores();
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
        AddPoints(10.0f);
        (*it)->Hit();
        addedPoints = true;

        // Yield
        Yield* pYield = new Yield((*it)->GetRect().getPosition());
        vecYield.push_back(pYield);

        pBall->SpeedUp(1.02f);
      }
    }

    // Yield hits paddle
    if(!vecYield.empty()){
      for(std::vector<Yield*>::iterator it = vecYield.begin(); it < vecYield.end(); it++){
        if((*it)->isActive() && (*it)->CheckCollision(pPaddle->GetRect())){
          (*it)->Hit();
          AddMultiplier();
        }
      }
    }
  }

  for(std::vector<Yield*>::iterator it = vecYield.begin(); it < vecYield.end(); it++){
    (*it)->Update(dTime);
  }
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

  if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !GameStarted && !gameOver){
    sf::Mouse mouse;
    sf::Vector2f mousePos = sf::Vector2f(float(mouse.getPosition().x),float(mouse.getPosition().y));
    StartGame(mousePos);
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
  amount *= multiplier;
  if(slowMotion)
    amount /= 2;
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

void Game::AddMultiplier() {
  multiplier++;
  multiplierTimer = 0.0f;
}

void Game::AddMultiplier(int k) {
  multiplier += k;
  multiplierTimer = 0.0f;
}

void Game::ReadHighscores (std::string filename) {
  Highscores.clear();
  std::ifstream ifs;
  ifs.open(filename.c_str());

  if(ifs.is_open()){
    std::string line;
    int i = 0;

    while(getline(ifs, line) && i < 10){
      Highscore hs;
      hs.nickname = line.substr (0, line.find_first_of(':'));
      hs.score = stoi(line.substr (line.find_first_of(':') + 1));

      Highscores.push_back(hs);

      i++;
    }
  }
  SortHighscores();
}

void Game::PrintHighscores () {
  std::cout << "Highscores :\n";
  for(unsigned int i = 0; i < Highscores.size(); i++){
    std::cout << Highscores[i].nickname << " : " << Highscores[i].score << std::endl;
  }
}

void Game::SortHighscores () {
  Highscore temp;
  for(unsigned int i = 1; i < Highscores.size(); i++){
    if(Highscores[i].score > Highscores[i - 1].score){
      temp = Highscores[i - 1];
      Highscores[i - 1] = Highscores[i];
      Highscores[i] = temp;
      SortHighscores();
    }
  }
}

bool Game::CheckIfHighscore(){
  if(Highscores.size() < 10 || points > Highscores.back().score)
    return true;
  return false;
}

void Game::SetNewHighscores (std::string name, std::string filename) {
  Highscore newHighscore;
  newHighscore.nickname = name;
  newHighscore.score = points; 
  Highscores.push_back(newHighscore);
  SortHighscores();

  std::ofstream highscoreFile;
  highscoreFile.open (filename, std::ofstream::trunc);
  for(unsigned int i = 0; i < Highscores.size(); i++){
    highscoreFile << std::string(Highscores[i].nickname + ":" + std::to_string(Highscores[i].score) + "\n");
  }
}

void Game::Render(sf::RenderWindow* pWindow, sf::Font font, std::string PName){

  sf::Text textPoints(txt, font);
  textPoints.setCharacterSize(40);
  textPoints.setPosition(sf::Vector2f(25.0f, 25.0f));
  if(slowMotion){
    textPoints.setString(textPoints.getString() + "\nSlowmotion");
  }
  if(!pause){
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
    }
}else
  {
    textPoints.setCharacterSize(40);
    textPoints.setColor(sf::Color::White);
    if (pause) {
      textAnimationTimer += dTime.asSeconds();

      textPoints.setString("Paused\n" + textPoints.getString());
      textPoints.setPosition(200, 200);
      if(fmod(textAnimationTimer, 2.0f) < 1.0f)
      {
        textPoints.setCharacterSize(41);
        textPoints.setColor(sf::Color::Blue);
      }
      else
      {
        textPoints.setColor(sf::Color::White);
      }
    }
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
  }if(gameOver && pPaddle->OutOfLives()){
    textPoints.setString("Game Over\nEnter nickname\n" + PName);
    if(!InputNameState){
      textPoints.setString(textPoints.getString() + "\nR to show highscores\n" + "Space to continue");
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
        bShowHighscores = true;
      }
      else{
        bShowHighscores = false;
      }
      if(bShowHighscores){
        textPoints.setString("");
        textPoints.setCharacterSize(22);
        for(unsigned int i = 0; i < Highscores.size(); i++){
          textPoints.setString(textPoints.getString() + Highscores[i].nickname + " " + std::to_string(Highscores[i].score) + "\n");
        }
        textPoints.setString(textPoints.getString() + "Space to continue");
      }
    }
  }

  pWindow->clear();


  for(std::vector<Yield*>::iterator it = vecYield.begin(); it < vecYield.end(); it++){
    if((*it)->isActive()){
      pWindow->draw((*it)->GetRect());
    }
  }


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
