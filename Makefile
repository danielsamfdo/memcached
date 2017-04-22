all:
	g++ -std=c++11 -I src/ -c src/main.cpp -lpthread
	g++ -std=c++11 -I src/ src/client/client.cpp -o client.o