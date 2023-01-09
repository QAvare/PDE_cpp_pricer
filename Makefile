all: main.cpp bs_price.o Matrix.o pde_pricer.o 
	g++ -Wall --std=c++17 -o bin/main main.cpp bin/bs_price.o bin/Matrix.o bin/pde_pricer.o

# main.o: main.cpp bs_price.o pde_pricer.o
# 	g++ -c -Wall --std=c++17 main.cpp -o bin/main.o -I

Matrix.o: Matrix.h Matrix.cpp
	g++ -c -Wall --std=c++17 Matrix.cpp -o bin/Matrix.o -I .

bs_price.o: bs_price.h bs_price.cpp
	g++ -c -Wall --std=c++17 bs_price.cpp -o bin/bs_price.o -I .

pde_pricer.o: pde_pricer.h pde_pricer.cpp
	g++ -c -Wall --std=c++17 pde_pricer.cpp -o bin/pde_pricer.o -I .

clean:
	rm -rf bin/*.o bin/main *.o