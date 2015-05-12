CFLAGS = -I ./include
LIB    = ./libggfonts32.so
#LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr this was com.
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids


asteroids: asteroids.cpp ppm.c
	g++ $(CFLAGS) asteroids.cpp -Wall -Wextra $(LFLAGS) -o asteroids
#asteroids2: asteroids2.cpp ppm.c log.c this was com.
#	g++ $(CFLAGS) asteroids2.cpp log.c -Wall -Wextra $(LFLAGS) -o asteroids2 this was com.

	




clean:
	rm -f asteroids 
	rm -f *.o



