all:
	g++ -fdiagnostics-color=always -g ../src/*.cpp ../src/*.c -I ../include/ -o main -lGL -lglfw -lm `pkg-config --cflags --libs opencv` 
	# g++ -g --std=c++17 -I ../include -L ../lib ../src/*.cpp ../src/glad.c -lglfw3dll -o main