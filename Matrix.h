#include <vector>
#include <cmath>

class Matrix
{
    public:
        Matrix();
        Matrix(int m, int n);
        int rows() const;
        int cols() const;
        double& operator() (int i, int j);
        Matrix Invert();
        int size() const;
        void print();


    private:
        int rows_;
        int cols_;
        double **data_;
};