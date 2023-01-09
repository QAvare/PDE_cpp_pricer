#include "Matrix.h"

class pde_pricer
{
private:
    double spot;
    double strike;
    double rate;
    double vol;
    double T;
    int lambda;
    int n;
    int m;

    Matrix time_matrix;
    Matrix x_matrix;
    Matrix u;
    Matrix P;
    Matrix V;
    Matrix Q;
    Matrix U;

public:
    pde_pricer(
        double spot,
        double strike,
        double rate,
        double vol,
        double T,
        int lambda,
        int n,
        int m
    );
    ~pde_pricer(){};

    void print();
    void initPricerCall(); //init pricer Matrix and limit conditions
    Matrix priceCall(); // 1step price

};
