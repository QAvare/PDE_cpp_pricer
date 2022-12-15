#include <math.h>
#include "utils.h"


double normal_cdf(double x){
    return (
        0.5 * erfc(-x * M_SQRT1_2)
    );
};
