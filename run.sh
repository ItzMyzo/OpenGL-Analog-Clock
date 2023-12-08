export LD_LIBRARY_PATH=/usr/local/lib64
g++ -O2 -Wall -Wextra -pedantic `pkg-config --cflags glfw3` src/*.cpp `pkg-config --static --libs glfw3` -lGLEW -lGLU -lGL -o Clock
./Clock