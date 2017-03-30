#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "game.hpp"

int main ()
{
  srand(time(nullptr));
  // Create the window
  int wWidth= 600;
  int wHeight = 640;
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode (wWidth, wHeight), "Arkanoid", sf::Style::Default, settings);
  window.setFramerateLimit(60);

  sf::Font font;
  font.loadFromFile("./Fonts/Regular.ttf");

  // Music
  //sf::Music menuMusic;
  //menuMusic.setPosition(0, 1, 10);
  //menuMusic.setLoop(true);
  //menuMusic.play();

  Game* pMyGame = new Game();
  bool slomoButtonPressed = false;
  bool pauseButtonPressed = false;
  std::string s;
  while (window.isOpen ())
  {
    sf::Event event;

    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed
          && event.key.code == sf::Keyboard::Escape))
      {
        std::cout << "Game halted!" << std::endl;
        window.close();
      }

      if (event.type == sf::Event::LostFocus){
        //menuMusic.pause();
      }

      if (event.type == sf::Event::GainedFocus){
        //menuMusic.play();
      }

      if(!pMyGame->InputState()) {
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S && !slomoButtonPressed)
        {
          pMyGame->SlowMotion();
          slomoButtonPressed = true;
        }
        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S)
        {
          slomoButtonPressed = false;
        }
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P && !pauseButtonPressed)
        {
          pMyGame->Pause();
          pauseButtonPressed = true;
        }
        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::P)
        {
          pauseButtonPressed = false;
        }
      }else{
        if (event.type == sf::Event::TextEntered){
          if(event.key.code == sf::Keyboard::BackSpace && s.size()!=0){
            s.pop_back();
            std::cout << s << std::endl;
          }
          else if(event.key.code == sf::Keyboard::Return && s.size()!=0){
            pMyGame->InputName(s);
            pMyGame->InputNameState = false;
            break;
          }
          else if (event.text.unicode < 128) {
            s.push_back((char)event.text.unicode);
            std::cout << s << std::endl;
          }
        }
      }
    }

    pMyGame->Update(event);
    pMyGame->Render(&window, font);
  }
  delete pMyGame;
  return 0;
}
