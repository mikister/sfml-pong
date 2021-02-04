.PHONY := all clean compile link

all: compile link clean

clean:
	rm main.o

compile:
	g++ -c main.cpp

link:
	g++ main.o -o pong -lsfml-graphics -lsfml-window -lsfml-system
