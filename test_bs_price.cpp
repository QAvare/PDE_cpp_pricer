#include <iostream>
#include "bs_price.h"

int main()
{
  double S = 100;  
  double K = 110;
  double q = 0;  
  double r = 0.05;  
  double sigma = 0.3; 
  double T = 1;
  char callPutFlag = 'c';


  std::cout << "Price of the call: " << blackScholes(callPutFlag, S, K,  r, q, sigma,  T) << std::endl;

  return 0;
}