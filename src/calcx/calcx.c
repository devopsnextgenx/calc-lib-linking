#include<stdio.h>
#include "calc/calc.h"

double addx(double a, double b) {
    printf("addx -> %f, %f\n", a, b);
    return add(a, b);
}

double subx(double a, double b) {
    printf("subx -> %f, %f\n", a, b);
    return subtract(a, b);
}

double mulx(double a, double b) {
    printf("mulx -> %f, %f\n", a, b);
    return multiply(a, b);
}

double divx(double a, double b) {
    printf("divx -> %f, %f\n", a, b);
    return divide(a, b);
}

double powx(double base, double exponent) {
    printf("powx -> %f, %f\n", base, exponent);
    return power(base, exponent);
}
double sqrtx(double value) {
    printf("sqrtx -> %f\n", value);
    return squareRoot(value);
}
void versionx(void) {
    printf("calcx version 1.0.0\n");
    version();
}

int main() {
    double a = 9.0, b = 3.0;
    printf("Add: %f\n", addx(a, b));
    printf("Subtract: %f\n", subx(a, b));
    printf("Multiply: %f\n", mulx(a, b));
    printf("Divide: %f\n", divx(a, b));
    printf("Power: %f\n", powx(a, b));
    printf("Square Root: %f\n", sqrtx(a));
    versionx();
    return 0;
}