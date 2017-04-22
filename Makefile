all:
	g++ -std=c++11 -I src/ src/server/server.cpp -o server.o
	g++ -std=c++11 -I src/ src/client/client.cpp -o client.o