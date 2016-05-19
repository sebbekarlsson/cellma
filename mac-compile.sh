g++ src/main.cpp `pkg-config --libs sdl2` -framework OpenGL -framework GLUT -lSDL2_image -lSDL2_ttf -g
./a.out
