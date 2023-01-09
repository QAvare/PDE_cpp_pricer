all: main.o Matrix.o bs_price.o
	g++ -o bin/main bin/main.o bin/bs_price.o bin/Matrix.o

main.o: main.cpp Matrix.o bs_price.o
	g++ -c -Wall --std=c++17 main.cpp -o bin/main.o

Matrix.o: Matrix.h Matrix.cpp
	g++ -c -Wall --std=c++17 Matrix.cpp -o bin/Matrix.o

bs_price.o: bs_price.h bs_price.cpp
	g++ -c -Wall --std=c++17 bs_price.cpp -o bin/bs_price.o

clean:
	rm -rf bin/*.o bin/main *.o