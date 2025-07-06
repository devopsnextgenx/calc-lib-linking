#include<stdio.h>
#include<math.h>
#include<string.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_hints.h>
#ifdef __linux__
#include <stdlib.h>  // For setenv on Linux
#endif
#include "calc/calc.h"
#include "graphics/graphics.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define WIDTH 900
#define HEIGHT 720

#define BLACK 0x00000000
#define SUN_COLOR 0xFF007FFF // Sun color yellow (ARGB format: 0xAARRGGBB)
#define EARTH_COLOR 0x0000FF00 // Earth color blue (ARGB format: 0xAARRGGBB)
#define MOON_COLOR 0xFFB2B2B2 // Moon color ash gray
#define RAY_COLOR 0xFF4D4D66 // Ray color (ARGB format: 0xAARRGGBB)

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
        printf("Initializing SDL for graphics...\n");
        
        // Clear any previous errors
        SDL_ClearError();
        
        // Set platform-specific video driver
        #ifdef _WIN32
        // Windows: use windows driver
        _putenv("SDL_VIDEODRIVER=windows");
        printf("Set SDL_VIDEODRIVER environment variable to 'windows'\n");
        if (!SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "windows")) {
            printf("Warning: Failed to set video driver hint to 'windows'\n");
        } else {
            printf("Successfully set video driver hint to 'windows'\n");
        }
        #elif defined(__linux__)
        // Linux: use x11 driver (or let SDL auto-detect)
        setenv("SDL_VIDEODRIVER", "x11", 1);
        printf("Set SDL_VIDEODRIVER environment variable to 'x11'\n");
        if (!SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11")) {
            printf("Warning: Failed to set video driver hint to 'x11'\n");
        } else {
            printf("Successfully set video driver hint to 'x11'\n");
        }
        #else
        // Other platforms: let SDL auto-detect the best driver
        printf("Using SDL auto-detection for video driver\n");
        #endif
        
        // Try to initialize SDL with video subsystem
        if (!SDL_Init(SDL_INIT_VIDEO)) {
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
            struct Circle sun(180, 100, 60); // Constructor initialization
            struct Circle earth(600, 350, 20); // Constructor initialization
            struct Circle moon(450, 400, 8); // Constructor initialization
            struct Ray rays[RAY_COUNT]; // Initialize array to zero
            
            // Moon orbital parameters
            double planet_angular_speed = 0.025; // Radians per frame (orbital speed)
            double moon_orbit_radius = 70.0; // Distance from earth center
            double moon_angle = 0.0; // Current orbital angle in radians
            double earth_angle = 0.0; // Current angle of earth in radians
            double earth_orbit_radius = 420.0; // Distance from sun center
            generateRays(sun, rays); // Generate rays for the sun

            SDL_Surface* surface = SDL_GetWindowSurface(window);
            struct Circle planets[PLANET_COUNT] = {earth, moon}; // Array of planets
            while (!quit) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_EVENT_QUIT) {
                        printf("Quit event received\n");
                        quit = 1;
                    }
                    else if (e.type == SDL_EVENT_WINDOW_RESIZED) {
                        printf("Window resized to %dx%d\n", e.window.data1, e.window.data2);
                        // Re-acquire surface after resize
                        surface = SDL_GetWindowSurface(window);
                        if (!surface) {
                            fprintf(stderr, "Failed to get window surface after resize: %s\n", SDL_GetError());
                        }
                    }
                    else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                        if (e.button.button == SDL_BUTTON_LEFT) {
                            double dx = e.button.x - sun.x;
                            double dy = e.button.y - sun.y;
                            double distance_squared = dx * dx + dy * dy;
                            double radius_squared = sun.r * sun.r;
                            bool grabSun = distance_squared <= radius_squared;
                            // Only grab the sun if the click is within its radius
                            if (grabSun) {
                                printf("Grabbed sun at (%f, %f)\n", (double)e.button.x, (double)e.button.y);
                                mouse_pressed = 1;
                                circle_placed = 1;
                                sun.x = e.button.x;
                                sun.y = e.button.y;
                                // Regenerate rays when sun moves
                                generateRays(sun, rays);
                            }
                        }
                    }
                    else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                        if (e.button.button == SDL_BUTTON_LEFT) {
                            mouse_pressed = 0;
                        }
                    }
                    else if (e.type == SDL_EVENT_MOUSE_MOTION) {
                        if (mouse_pressed && circle_placed) {
                            sun.x = e.motion.x;
                            sun.y = e.motion.y;
                            // Regenerate rays when sun moves
                            generateRays(sun, rays);
                        }
                    }
                }
                
                // Update moon orbital position around earth
                moon_angle += planet_angular_speed;
                if (moon_angle >= 2 * M_PI) {
                    moon_angle -= 2 * M_PI; // Keep angle in range [0, 2π)
                }
                moon.x = earth.x + moon_orbit_radius * cos(moon_angle);
                moon.y = earth.y + moon_orbit_radius * sin(moon_angle);
                
                earth_angle += planet_angular_speed/12;
                if (earth_angle >= 2 * M_PI) {
                    earth_angle -= 2 * M_PI; // Keep angle in range [0, 2π)
                }
                earth.x = sun.x + earth_orbit_radius * cos(earth_angle);
                earth.y = sun.y + earth_orbit_radius * sin(earth_angle);

                planets[0] = earth; // Update earth position
                planets[1] = moon; // Update moon position
                if (surface) {
                    SDL_FillSurfaceRect(surface, NULL, BLACK);
                    drawSunrays(surface, sun, rays, RAY_COLOR, planets);
                    drawCircle(surface, sun, SUN_COLOR);
                    for (int i = 0; i < PLANET_COUNT; i++) {
                        drawCircle(surface, planets[i], (i == 0) ? EARTH_COLOR : MOON_COLOR);
                    }
                    SDL_UpdateWindowSurface(window);
                } else {
                    fprintf(stderr, "Failed to get window surface: %s\n", SDL_GetError());
                }
                // Render here if needed
                SDL_Delay(20); // ~60 FPS
            }
        }
        
        
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }
    
    printf("Usage: %s [--version|--calc|--graphics]\n", argv[0]);
    return 0;
}