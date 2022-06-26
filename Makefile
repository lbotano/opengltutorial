build: main.cpp
	g++ main.cpp -o run -lglad -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

debug: main.cpp
	g++ -g main.cpp -o run -lglad -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
