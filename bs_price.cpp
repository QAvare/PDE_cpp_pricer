#include <cmath>
#include <algorithm>

double cdf(double x)
{
    return 0.5 * std::erfc(-x / std::sqrt(2));
}

double blackScholes(char callPutFlag, double S, double K, double r, double q, double sigma, double T)
{
    double d1 = (std::log(S / K) + (r - q + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    double price;

    if (callPutFlag == 'c')
    {
        price = S * std::exp(-q * T) * cdf(d1) - K * std::exp(-r * T) * cdf(d2);
    }
    else if (callPutFlag == 'p')
    {
        price = K * std::exp(-r * T) * cdf(-d2) - S * std::exp(-q * T) * cdf(-d1);
    }
    else
    {
        throw "Invalid callPutFlag";
    }

    return price;
}
