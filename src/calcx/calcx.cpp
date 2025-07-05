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
        SDL_ClearError();
        printf("Initializing SDL for graphics...\n");
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
            return 1;
        }
        int num_drivers = SDL_GetNumVideoDrivers();
        printf("Available video drivers: %d\n", num_drivers);
        for (int i = 0; i < num_drivers; i++) {
            printf("  - %s\n", SDL_GetVideoDriver(i));
        }

        const char* current_driver = SDL_GetCurrentVideoDriver();
        printf("Current video driver: %s\n", current_driver ? current_driver : "None");
    
        SDL_Window* window = SDL_CreateWindow("CalcX::Graphics", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
        if (window == NULL) {
            fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
            SDL_Quit();
            return 1;
        } else {
            printf("SDL_CreateWindow succeeded\n");

            SDL_Event e;
            int mouse_pressed = 0;
            int quit = 0;
            int circle_placed = 0;
            Circle circle = {600, 400, 60}; // C-style struct initialization

            SDL_Surface* surface = SDL_GetWindowSurface(window);
            while (!quit) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_EVENT_QUIT) {
                        printf("Quit event received\n");
                        quit = 1;
                    }
                    else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                        if (e.button.button == SDL_BUTTON_LEFT) {
                            mouse_pressed = 1;
                            circle_placed = 1;
                            circle.x = e.button.x;
                            circle.y = e.button.y;
                            printf("Circle placed at (%f, %f)\n", circle.x, circle.y);
                        }
                    }
                    else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                        if (e.button.button == SDL_BUTTON_LEFT) {
                            mouse_pressed = 0;
                            printf("Mouse released\n");
                        }
                    }
                    else if (e.type == SDL_EVENT_MOUSE_MOTION) {
                        if (mouse_pressed && circle_placed) {
                            circle.x = e.motion.x;
                            circle.y = e.motion.y;
                            // Uncomment for verbose mouse tracking
                            // printf("Circle moved to (%f, %f)\n", circle.x, circle.y);
                        }
                    }
                }
                if (surface) {
                    Uint32 black = 0x00000000; // Black color
                    SDL_FillSurfaceRect(surface, NULL, black);
                    Uint32 color = 0xffffffff; // Red color
                    drawCircle(surface, circle, color);
                    SDL_UpdateWindowSurface(window);
                } else {
                    fprintf(stderr, "Failed to get window surface: %s\n", SDL_GetError());
                }
                // Render here if needed
                SDL_Delay(16); // ~60 FPS
            }
        }
        
        
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }
    
    printf("Usage: %s [--version|--calc|--graphics]\n", argv[0]);
    return 0;
}