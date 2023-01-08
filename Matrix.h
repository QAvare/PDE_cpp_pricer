#include <vector>
#include <cmath>

class Matrix
{
public:
    Matrix();
    Matrix(int m, int n);
    
    int rows() const;
    int cols() const;
    int size() const;
    
    Matrix Invert();
    Matrix dot(Matrix m);
    void print();
    
    Matrix operator*(double x);
    double &operator()(int i, int j);

private:
    int rows_;
    int cols_;
    double **data_;
};
