# define libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif
LIB2 = libim/libim.a jpeg/libjpeg.a

# define makefile targets
CC = g++ -std=c++11 -Wall -Wno-deprecated-declarations -O3
BIN = balls balls2 balls3 balls4 bezier bitmap building building2 building3 callback clip clip2 cube cube2 cube3 cube4 curve disk gasket2d gasket3d gouraud lagrange line_split line_test mandelbrot mandelbrot2 object object2 object3 object4 office office2 office3 office4 pong rectangles render render2 render3 square square2 surface surface2 surface3 surface4 tetris texture texture2 texture3 texture4 texture5 tree whack_a_mole image penny

all: $(LIB2) $(BIN)


clean:
	/bin/rm -f $(LIB2) $(BIN)

jpeg/libjpeg.a:
	cd jpeg;make;cd ..

libim/libim.a:
	cd libim;make;cd ..

tetris: tetris.cpp
	$(CC) -o tetris tetris.cpp $(LIB)
