#include<stdio.h>
#include<math.h>
#include<cmath>
#include<SDL3/SDL.h>
#include "graphics/graphics.h"

// Define M_PI if not already defined (Windows compatibility)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// https://www.youtube.com/watch?v=2BLRLuczykM
namespace graphics {
    void drawCircle(SDL_Surface* surface, struct Circle circle, Uint32 color) {
        double xc = circle.x;
        double yc = circle.y;
        double r = circle.r;
        
        double rsq = pow(r, 2);
        double x_end = xc + r;
        double y_end = yc + r;
        int pixel_counter_x = 0;
        int pixel_counter_y = 0;
        for (double x = xc - r; x <= x_end; x++) {
            pixel_counter_x++;
            pixel_counter_y = 0; // Reset y counter for each x
            for (double y = yc - r; y <= y_end; y++) {
                pixel_counter_y++;
                if (pow(x - xc, 2) + pow(y - yc, 2) <= rsq) {
                    SDL_Rect pixel = {(int)x, (int)y, 1, 1};
                    SDL_FillSurfaceRect(surface, &pixel, color);
                }
            }
        }
        //printf("Circle drawn at (%f, %f) with radius %f, pixels drawn: %d x %d\n", xc, yc, r, pixel_counter_x, pixel_counter_y);
    }

    void generateRays(struct Circle sun, struct Ray rays[RAY_COUNT]) {
        double angle_step = (2.0 * M_PI) / RAY_COUNT; // Use radians throughout
        for (int i = 0; i < RAY_COUNT; i++) {
            double angle = i * angle_step; // Angle in radians
            rays[i].x = sun.x;
            rays[i].y = sun.y;
            rays[i].a = angle;
        }
    }

    void drawSunrays(SDL_Surface* surface, struct Circle sun, struct Ray rays[RAY_COUNT], Uint32 color, struct Circle planets[PLANET_COUNT]) {
        for (int i = 0; i < RAY_COUNT; i++) {
            struct Ray ray = rays[i];
            bool is_outside_window = false;
            bool is_blocked = false;
            int step = 1; // Step size for ray length
            double xc = ray.x;
            double yc = ray.y;
            while (!is_outside_window && !is_blocked) {
                double angle = ray.a; // Convert angle to radians
                xc += cos(angle) * step; // Calculate end point of the ray
                yc += sin(angle) * step; // Calculate end point of the ray
                SDL_Rect pixel = {(int)xc, (int)yc, 1, 1};
                SDL_FillSurfaceRect(surface, &pixel, color); // Draw the ray
                if (xc < 0 || xc >= surface->w || yc < 0 || yc >= surface->h) {
                    is_outside_window = true; // Ray is outside the window
                }


                for (int j = 0; j < PLANET_COUNT; j++) {
                    struct Circle planet = planets[j];
                    double dx = xc - planet.x;
                    double dy = yc - planet.y;
                    double distance_squared = dx * dx + dy * dy;
                    double radius_squared = planet.r * planet.r;
                    if (distance_squared <= radius_squared) {
                        is_blocked = true; // Ray is blocked by a planet
                        break; // No need to check other planets
                    }
                }
            }
        }
    }
}