#include "Matrix.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const double EPS = 1e-9;

// Matrix class
Matrix::Matrix() 
{
  int rows_(0);
  int cols_(0);
  double **data_;
  //data_(m, vector<double>(n, 0)) // m * vector(n * 0)
}


  // Constructor to create an m x n matrix with all elements initialized to 0
  Matrix::Matrix(int m, int n)
  {
    rows_ = m;
    cols_ = n;
    //double data_(rows_, vector<double>(cols_));
    data_ = new double*[rows_];
    for(int i = 0; i < rows_; i++)
      data_[i] = new double[cols_];
  }


  // Access element at ith row and jth column
  double& Matrix::operator() (int i, int j) {
    return data_[i][j];
  }

  // Return the number of rows in the matrix
  int Matrix::rows() const { return rows_; }

  // Return the number of columns in the matrix
  int Matrix::cols() const { return cols_; }

  // Invert the matrix
  Matrix Matrix::Invert() 
  {
    // Check if the matrix is square
    if (rows_ != cols_) {
      cerr << "Error: Matrix is not square." << endl;
      exit(1);
    }

    // Create an identity matrix of the same size as the original matrix
    Matrix I(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
      I(i, i) = 1;
    }

    // Use Gaussian elimination to compute the inverse
    Matrix A = *this;  // Make a copy of the original matrix
    for (int i = 0; i < rows_; i++) {
      // Find the pivot element
      int pivot = i;
      for (int j = i + 1; j < rows_; j++) {
        if (abs(A(j, i)) > abs(A(pivot, i))) {
          pivot = j;
        }
      }

      // Swap rows
      if (pivot != i) {
        swap(A(i, i), A(pivot, i));
        swap(I(i, i), I(pivot, i));
      }

      // Eliminate the pivot element from the other rows
      for (int j = i + 1; j < rows_; j++) {
        double factor = A(j, i) / A(i, i);
        for (int k = i; k < cols_; k++) {
          A(j, k) -= A(i, k) * factor;
          I(j, k) -= I(i, k) * factor;
        }
      }
    }

    // Back-substitute to solve for the inverse
    for (int i = rows_ - 1; i >= 0; i--) {
      double factor = A(i, i);
      for (int j = 0; j < i; j++) {
        A(j, i) -= A(i, j) * A(j, i) / factor;
        I(j, i) -= A(i, j) * I(j, i) / factor;
      }
      I(i, i) /= factor;
    }

    return I;
  }

  int Matrix::size() const{
    return rows_;
  }

  void Matrix::print(){
    for (int i = 0; i < rows_; i++)
    {
      for (int j = 0; j < cols_; j++)
      {
        cout << data_[i][j] << " ";
      }
      cout << endl;
    }
  }
