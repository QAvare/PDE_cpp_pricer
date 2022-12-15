all: main.o utils.o
	g++ -o bin/main bin/main.o bin/utils.o

main.o: main.cpp utils.o
	g++ -c -Wall --std=c++17 main.cpp -o bin/main.o

utils.o: utils.h utils.cpp
	g++ -c -Wall --std=c++17 utils.cpp -o bin/utils.o

clean:
	rm -rf bin/*.o bin/main *.o