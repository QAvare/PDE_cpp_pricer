/*

    CPP pricer PDE - Msc 203

    Virgile Brian - Quentin Avare - Clément Rivière

*/
#include <iostream>
#include "utils.h"
#include "Matrix.h"

using namespace std;


int main(int argc, char **argv){

    Matrix data = Matrix(3, 3);
    for(int i=0; i<data.size(); ++i)
    {
        data(i,i) = 1;
    }
    data.print();

    return 0;
};