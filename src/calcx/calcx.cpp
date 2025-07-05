#include<stdio.h>
#include "calc/calc.h"
#include "graphics/graphics.h"
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL.h>

#define WIDTH 640
#define HEIGHT 480

using namespace graphics;
using namespace calc;

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

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--version") == 0) {
        versionx();
        return 0;
    }
    if (argc > 1 && strcmp(argv[1], "--calc") == 0) {
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
    if (argc > 1 && strcmp(argv[1], "--graphics") == 0) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
            return 1;
        }
        
        SDL_Window* window = SDL_CreateWindow("CalcX::Graphics", WIDTH, HEIGHT, 0);
        if (!window) {
            fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        } else {
            printf("SDL_CreateWindow succeeded\n");
            getchar(); // Wait for user input before closing
        }
        
        // SDL_Surface* surface = SDL_GetWindowSurface(window);
        // if (surface) {
        //     Circle circle = {320, 240, 100}; // C-style struct initialization
        //     Uint32 color = SDL_MapRGB(surface->format, 255, 0, 0); // Red color
        //     drawCircle(&surface, circle, color);
        //     SDL_UpdateWindowSurface(window);
        //     SDL_Delay(5000); // Show for 5 seconds
        // } else {
        //     fprintf(stderr, "Failed to get window surface: %s\n", SDL_GetError());
        // }
        
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }
    
    printf("Usage: %s [--version|--calc|--graphics]\n", argv[0]);
    return 0;
}