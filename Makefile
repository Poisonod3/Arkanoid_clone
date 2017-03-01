TARGETS= main.o gameobject.o paddle.o ball.o game.o tile.o level.o
CFLAGS= -Wall -pedantic -std=c++11

all : $(TARGETS)
	g++ -o Arkanoid $(TARGETS) -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

main.o : main.cpp
		g++ -c $(CFLAGS) main.cpp

gameobject.o : gameobject.cpp gameobject.hpp
		g++ -c $(CFLAGS) gameobject.cpp

paddle.o : paddle.cpp paddle.hpp
		g++ -c $(CFLAGS) paddle.cpp

ball.o : ball.cpp ball.hpp
		g++ -c $(CFLAGS) ball.cpp

game.o : game.cpp game.hpp
		g++ -c $(CFLAGS) game.cpp

tile.o : tile.cpp tile.hpp
		g++ -c $(CFLAGS) tile.cpp

level.o : level.cpp level.hpp
		g++ -c $(CFLAGS) level.cpp

clean :
	rm -f *.o Arkanoid
