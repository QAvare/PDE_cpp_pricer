/*

    CPP pricer PDE - Msc 203

    Virgile Brian - Quentin Avare - Clément Rivière

*/
#include <iostream>
#include "Matrix.h"
#include "bs_price.h"
#include "pde_pricer.h"

using namespace std;

int main(int argc, char **argv)
{

    // Matrix test;
    Matrix A = Matrix(2, 2);
    Matrix B = Matrix(2, 2);

    B(0, 0) = 1;
    B(0, 1) = 0;
    B(1,0) = 0;
    B(1, 1) = 1;
    
    A(0, 0) = 1;
    A(0, 1) = 0;
    A(1, 0) = 0;
    A(1, 1) = 1;
   

    Matrix C = B.dot(A);
    cout << "\n"
         << endl;
    //A.print();
    C=C.Invert();
    //C.print();

    // BS pricer test
    double S = 100;
    double K = 110;
    double q = 0;
    double r = 0.005;
    double sigma = 0.003;
    double T = 1;
    char callPutFlag = 'c';

    std::cout << "Price of the call: " << blackScholes(callPutFlag, S, K, r, q, sigma, T) << std::endl;

    pde_pricer pricer = pde_pricer(
        S,
        K,
        r,
        sigma,
        T,
        4,
        100,
        100);

    pricer.initPricerCall();
    cout << "\nPricer characteristics : " << endl;
    pricer.print();
    Matrix u = pricer.priceCall();

    cout << "\nmatrice U a t=0 \n";
    u.print();

    return 0;
};