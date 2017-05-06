all:
	g++ -std=c++11 -I src/ src/main.cpp -o main.o -lpthread
	g++ -std=c++11 -I src/ src/client/client.cpp -o client.o
