all: main.o
	g++ -o bin/main bin/main.o

main.o : main.cpp
	g++ -c -Wall --std=c++17 main.cpp -o bin/main.o

clean:
	rm -rf bin/*.o bin/main *.o