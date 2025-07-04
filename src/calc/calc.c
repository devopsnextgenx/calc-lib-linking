#include <stdio.h>
#include <math.h>
#include "calc/calc.h"

double add(double a, double b) {
    printf("calc::Adding %f and %f\n", a, b);
    return a + b;
}

double subtract(double a, double b) {
    printf("calc::Subtracting %f from %f\n", b, a);
    return a - b;
}

double multiply(double a, double b) {
    printf("calc::Multiplying %f and %f\n", a, b);
    return a * b;
}

double divide(double a, double b) {
    printf("calc::Dividing %f by %f\n", a, b);
    if (b == 0) {
        printf("Error: Division by zero is not allowed.\n");
        return 0; // or NAN
    }
    return a / b;
}

double power(double base, double exponent) {
    printf("calc::Calculating %f raised to the power of %f\n", base, exponent);
    return pow(base, exponent);
}

double squareRoot(double value) {
    printf("calc::Calculating square root of %f\n", value);
    if (value < 0) {
        printf("Error: Square root of negative number is not allowed.\n");
        return 0; // or NAN
    }
    return sqrt(value);
}

void version(void) {
    printf("calc version 1.0.0\n");
}