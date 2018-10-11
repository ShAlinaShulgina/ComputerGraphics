CPP = g++

LIBS = -lglut -lGLU -lGL

all : main

main: main.o
	$(CPP) main.o -o main $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp

clean:
	rm -rf main.o main
