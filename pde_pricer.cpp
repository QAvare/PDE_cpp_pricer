#include "pde_pricer.h"
#include "math.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

// utils functions

Matrix treshold(Matrix u, double lower, double upper)
{
    Matrix uu = u;
    for (int j = 0; j < uu.rows(); j++)
    {
        if (uu(j, 0) < -lower )
        {
            uu(j, 0) = lower;
        }
        if (abs(uu(j, 0)) > upper)
        {
            uu(j, 0) = upper;
        }
    }
    return uu;
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

    this->theta = 0.5;

    this->time_matrix = Matrix(1, n + 1);
    this->x_matrix = Matrix(1, m + 1);
    this->dx = Matrix(1, m + 1);

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

    cout << "The x array: \n";
    x_matrix.print();
    cout << "The V matrix: \n";
    V.print();
    cout << "The U array: \n";
    U.print();
    cout << "\n";
}

Matrix pde_pricer::compute_x()
{
    Matrix x(1, m + 1);
    for (int i = 0; i <= m; i++)
    {
        x(0, i) = - lambda * vol + (2 * lambda * vol / m) * i;
    }
    return x;
}

Matrix pde_pricer::compute_dx()
{
    Matrix x = this->dx;
    double d = 1.0 / (m + 1);
    for (int i = 0; i <= m; i++)
    {
        x(0, i) = (lambda * vol) * d;
    }
    return x;
}

void pde_pricer::initPricerCall()
{
    x_matrix = compute_x(); // init x array
    dx = compute_dx(); // init dx array

    for (int j = 0; j < time_matrix.cols(); j++)
    {
        time_matrix(0, j) = (T / n + 1) * j;
    }
    // U matrix
    for (int k = 0; k < m - 1; k++)
    {
        double F = spot * exp(x_matrix(0, k) + rate * T);
        U(k, 0) = max(F - strike, 0.0);
    }
    // V matrix
    compute_V(T);
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
    double b = -0.5 * pow(vol, 2);
    double c = -b;
    for (int j = 0; j < m - 1; j++)
    {
        P(j, j) = -(n / T) - (2 * theta * c / (dx(0, j) * dx(0, j + 1)));
        if (j < m - 2)
        {
            P(j, j + 1) = (b / (dx(0, j) + dx(0, j + 1))) + (2 * theta * c / (dx(0, j) * dx(0, j + 1)));
            P(j + 1, j) = -(b / (dx(0, j + 1) + dx(0, j + 2))) + (2 * theta * c / (dx(0, j + 1) * dx(0, j + 2)));
        }
    }
}

void pde_pricer::compute_Q()
{
    double b = -0.5 * pow(vol, 2);
    double c = -b;
    for (int j = 0; j < m - 1; j++)
    {
        Q(j, j) = (n / T) - (2 * (1 - theta) * c) / (dx(0, j) * dx(0, j + 1));
        if (j < m - 2)
        {
            Q(j, j + 1) = (1 - theta) * c / (dx(0, j) * dx(0, j + 1));
            Q(j + 1, j) = (1 - theta) * c / (dx(0, j + 1) * dx(0, j + 2));
        }
    }
}

void pde_pricer::compute_V(double t)
{
    double b = -0.5 * pow(vol, 2);
    double c = -b;

    V(0, 0) = 0;
    V(m - 2, 0) = ((-b / (dx(0, m - 2) + dx(0, m - 1))) + 2 * theta * c / (dx(0, m - 2) * dx(0, m - 1))) * (spot * exp(x_matrix(0, m) + rate*(T - t - T/n)) - strike*exp(-(T - t - T/n) * rate)) + (((1 - theta) * c) / (dx(0, m - 2) * dx(0, m - 1))) * (spot * exp(x_matrix(0, m) /*+ rate*T*/) - strike*exp((-T + t) * rate));
}

Matrix pde_pricer::priceCall()
{
    // initPricerCall();
    U = getNextU();

    for (int i = n; i >= 0; i--)
    {
        double t = time_matrix(0, i);
        x_matrix = compute_x();
        compute_P();
        compute_Q();
        compute_V(t);
        U = getNextU();
        // for stability:
        U = treshold(U, 0.0001, 100000);
    }
    return U;
}
