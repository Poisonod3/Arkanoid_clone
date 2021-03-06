CC = g++
OBJECTS= main.o gameobject.o paddle.o ball.o game.o tile.o level.o yield.o
CFLAGS= -Wall -pedantic -std=c++11 -g

BUILDDIR= build

all : $(OBJECTS)
	$(CC) -o $(BUILDDIR)/Arkanoid $(OBJECTS) -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

main.o : main.cpp
	$(CC) -c $(CFLAGS) main.cpp

gameobject.o : gameobject.cpp gameobject.hpp
	$(CC) -c $(CFLAGS) gameobject.cpp

paddle.o : paddle.cpp paddle.hpp
	$(CC) -c $(CFLAGS) paddle.cpp

ball.o : ball.cpp ball.hpp
	$(CC) -c $(CFLAGS) ball.cpp

game.o : game.cpp game.hpp
	$(CC) -c $(CFLAGS) game.cpp

tile.o : tile.cpp tile.hpp
	$(CC) -c $(CFLAGS) tile.cpp

level.o : level.cpp level.hpp
	$(CC) -c $(CFLAGS) level.cpp

yield.o : yield.cpp yield.hpp
	$(CC) -c $(CFLAGS) yield.cpp
clean :
	rm -f *.o Arkanoid
