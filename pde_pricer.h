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
    
    double theta;

    Matrix time_matrix;
    Matrix x_matrix;
    Matrix dx;
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

    void compute_P();
    void compute_Q();
    void compute_V(double t);

    Matrix compute_x(); // compute x
    Matrix compute_dx(); // compute dx
    Matrix getNextU(); // one step get U(i+1)
    Matrix priceCall();

};
