#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
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

  // Music
  //sf::Music menuMusic;
  //menuMusic.setPosition(0, 1, 10);
  //menuMusic.setLoop(true);
  //menuMusic.play();
  
  Game* pMyGame = new Game();
  while (window.isOpen ())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape){
        std::cout << "Game halted!" << std::endl;
        window.close();
      }

      if (event.type == sf::Event::LostFocus){
        //menuMusic.pause();
      }

      if (event.type == sf::Event::GainedFocus){
        //menuMusic.play();
      }
    }

    pMyGame->Update(event);
    pMyGame->Render(&window);
  }
  return 0;
}
