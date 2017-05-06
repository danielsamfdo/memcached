all:
	g++ -std=c++11 -I src/ src/main.cpp -lpthread -o main.o
	g++ -std=c++11 -I src/ src/client/client.cpp -o client.o
