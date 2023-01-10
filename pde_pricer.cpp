#include "pde_pricer.h"
#include "math.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

// utils functions

double get_dx(Matrix x, int j)
{
    if (x.cols() < j || j < 0)
    {
        exit(1);
    }
    return x(0, j + 1) - x(0, j);
}

// --------------- //

pde_pricer::pde_pricer(
    double spot,
    double strike,
    double rate,
    double vol,
    double T,
    int lambda,
    int n,
    int m)
{
    this->spot = spot;
    this->strike = strike;
    this->rate = rate;
    this->vol = vol;
    this->T = T;
    this->lambda = lambda;
    this->n = n;
    this->m = m;

    this->time_matrix = Matrix(1, n);
    this->x_matrix = Matrix(1, m);

    this->P = Matrix(m - 1, m - 1);
    this->Q = Matrix(m - 1, m - 1);
    this->V = Matrix(m - 1, 1);
    this->U = Matrix(m - 1, 1);
}

void pde_pricer::print()
{
    cout << "spot " << this->spot << endl;
    cout << "strike " << this->strike << endl;
    cout << "rate " << this->rate << endl;
    cout << "vol " << this->vol << endl;
    cout << "T " << this->T << endl;
    cout << "lambda " << this->lambda << endl;
    cout << "n " << this->n << endl;
    cout << "m " << this->m << endl;

    //x_matrix.print();
    //time_matrix.print();
    //U.print();
    //Q.print();
    //P.print();
    V.print();
    cout << "\n";
}

Matrix pde_pricer::x_t(double t)
{
    Matrix x(1, m);
    for (int i = 0; i < m; i++)
    {
        float d = (float)i / (m-1);
        x(0, i) = -lambda*vol + (2*lambda*vol)*d;
    }
    return x;
}

void pde_pricer::initPricerCall()
{
    double b = 0.5 * pow(vol, 2);
    double c = -b;
    double theta = 0.5;

    x_matrix = x_t(T); // init x array

    for (int j = 0; j < n; j++)
    {
        time_matrix(0, j) = (T / n) * j;
    }
    // U matrix
    for (int k = 0; k < m - 1; k++)
    {
        double F = spot * exp(x_matrix(0, k) + rate * T);
        U(k, 0) = max(F - strike, 0.0);
        // U(k, 0) = exp(-rate * T) * max(F - strike, 0.0);
        // cout << F - strike;
    }
    // V matrix
    // V(0, 0) = 0;
    // V(m - 2, 0) = (b / (get_dx(x_matrix, m - 2) + get_dx(x_matrix, m - 1))) + (2 * theta * c / (get_dx(x_matrix, m - 2) * get_dx(x_matrix, m - 1))) * spot * exp(x_matrix(0, m)) - exp(-rate) + (((1 - theta) * c) / (get_dx(x_matrix, m - 2) * get_dx(x_matrix, m - 1))) * spot * exp(x_matrix(0, m));
    compute_V(T);
    U.print();
    // Q matrix
    compute_Q();

    // P matrix
    compute_P();
}

Matrix pde_pricer::getNextU()
{
    Matrix invP = -1.0 * P.Invert();
    Matrix nextU = invP.dot(Q.dot(U) + V);
    return nextU;
}

void pde_pricer::compute_P()
{
    double b = 0.5 * pow(vol, 2);
    double c = -b;
    for (int j = 0; j < m - 1; j++)
    {
        P(j, j) = -(n / T) - (2 * theta * c / (get_dx(x_matrix, j) * get_dx(x_matrix, j + 1)));
        if (j < m - 2)
        {
            P(j, j + 1) = (b / (get_dx(x_matrix, j) + get_dx(x_matrix, j + 1))) + (2 * theta * c / (get_dx(x_matrix, j) * get_dx(x_matrix, j + 1)));
            P(j + 1, j) = -(b / (get_dx(x_matrix, j + 1) + get_dx(x_matrix, j + 2))) + (2 * theta * c / (get_dx(x_matrix, j + 1) * get_dx(x_matrix, j + 2)));
        }
    }
}

void pde_pricer::compute_Q()
{
    double b = -0.5 * pow(vol, 2);
    double c = -b;
    for (int j = 0; j < m - 1; j++)
    {
        Q(j, j) = (n / T) - (2 * (1 - theta) * c) / (get_dx(x_matrix, j) * get_dx(x_matrix, j + 1));
        if (j < m - 2)
        {
            Q(j, j + 1) = (1 - theta) * c / (get_dx(x_matrix, j) * get_dx(x_matrix, j + 1));
            Q(j + 1, j) = (1 - theta) * c / (get_dx(x_matrix, j + 1) * get_dx(x_matrix, j + 2));
        }
    }
}

void pde_pricer::compute_V(double t)
{
    double b = -0.5 * pow(vol, 2);
    double c = -b;

    V(0, 0) = 0;
    V(m - 2, 0) = ((-b / (get_dx(x_matrix, m - 2) + get_dx(x_matrix, m - 1))) + 2 * theta * c / (get_dx(x_matrix, m - 2) * get_dx(x_matrix, m - 1))) * (spot * exp(x_matrix(0, m) + rate*(T-T/n)) - strike*exp(-(T - T/n) * rate)) + (((1 - theta) * c) / (get_dx(x_matrix, m - 2) * get_dx(x_matrix, m - 1))) * (spot * exp(x_matrix(0, m) + rate*T) - strike*exp(-T*rate));
}

Matrix pde_pricer::priceCall()
{
    // initPricerCall();
    // Matrix nextU(m - 1, 1);
    U = getNextU();

    for (int i = m - 1; i >= 0; i--)
    {
        double t = time_matrix(0, i);
        x_matrix = x_t(t);
        compute_P();
        compute_Q();
        compute_V(t);
        U = getNextU();
    }
    return U;
}
