#include "mathlib.h"

#include <math.h>

double Abs(double x) {
    return x > 0 ? x : -x;
}

double Sqrt(double x) {
    if (x < 0) {
        return nan("nan");
    }

    double old = 0.0;
    double new = 1.0;

    while (Abs(old - new) > EPSILON) {
        old = new;
        new = 0.5 * ((old + x / old));
    }

    return new;
}

double Sin(double x) {
    if (x == 0.0) {
        return 0.0;
    } //sin at 0 is 0
    double normed = x < 0 ? (fmod(x, 2 * M_PI) + 2 * M_PI)
                          : fmod(x, 2 * M_PI); //sets x to be between 0 and 2pi
    double term = normed; //sets the 0th term in the series to be x
    double sum = term; //initializes the sum
    double n = 1.0; //we handle the 0th term manually
    double sign = 1.0; // (-1)^n at n = 0 is 1
    double factorial = 1.0; // (2n+1)! at n = 0 is 1
    double exponent = term; // x^(2n+1) at n=0 is x
    while (Abs(term) > EPSILON) {
        factorial *= (2 * n + 1) * (2 * n);
        exponent *= normed * normed;
        sign *= -1;

        term = (sign * exponent) / factorial;
        sum += term;
        n++;
    }
    return sum;
}

double Cos(double x) {
    if (x == 0.0) {
        return 1.0;
    } //cos at 0 is 1
    double normed = x < 0 ? (fmod(x, 2 * M_PI) + 2 * M_PI)
                          : fmod(x, 2 * M_PI); //sets x to be between 0 and 2pi
    double term = 1; //sets the 0th term in the series to be 1
    double sum = term; //initializes the sum
    double n = 1.0; //we handle the 0th term manually
    double sign = 1.0; // (-1)^n at n = 0 is 1
    double factorial = 1.0; // (2n)! at n = 0 is 1
    double exponent = 1; // x^(2n) at n=0 is 1
    while (Abs(term) > EPSILON) {
        factorial *= (2 * n - 1) * (2 * n);
        exponent *= normed * normed;
        sign *= -1;

        term = (sign * exponent) / factorial;
        sum += term;
        n++;
    }
    return sum;
}

double Tan(double x) {
    double normed = x < 0 ? (fmod(x, 2 * M_PI) + 2 * M_PI) : fmod(x, 2 * M_PI);
    return Sin(normed) / Cos(normed);
}
