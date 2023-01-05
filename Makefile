all: main.o utils.o Matrix.o
	g++ -o bin/main bin/main.o bin/utils.o bin/Matrix.o

main.o: main.cpp utils.o Matrix.o
	g++ -c -Wall --std=c++17 main.cpp -o bin/main.o

utils.o: utils.h utils.cpp
	g++ -c -Wall --std=c++17 utils.cpp -o bin/utils.o

Matrix.o: Matrix.h Matrix.cpp
	g++ -c -Wall --std=c++17 Matrix.cpp -o bin/Matrix.o

class_options.o: class_options.h class_options.cpp
	g++ -c -Wall --std=c++17 class_options.cpp -o bin/class_options.o

clean:
	rm -rf bin/*.o bin/main *.o
