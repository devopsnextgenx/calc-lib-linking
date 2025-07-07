#include<stdio.h>
#include<math.h>
#include<cmath>
#include<SDL3/SDL.h>
#include "graphics/graphics.h"
#include <algorithm>
#include <iostream>

// Define M_PI if not already defined (Windows compatibility)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// https://www.youtube.com/watch?v=2BLRLuczykM
namespace graphics {
    void generateRays(Circle sun, struct Ray rays[RAY_COUNT]) {
        double angle_step = (2.0 * M_PI) / RAY_COUNT; // Use radians throughout
        for (int i = 0; i < RAY_COUNT; i++) {
            double angle = i * angle_step; // Angle in radians
            rays[i].x = sun.getX();
            rays[i].y = sun.getY();
            rays[i].a = angle;
        }
    }

    void drawRays(SDL_Surface* surface, Circle sun, struct Ray rays[RAY_COUNT], Uint32 color, Circle planets[PLANET_COUNT]) {
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

                // Check if the ray intersects with any planet
                for (int j = 0; j < PLANET_COUNT; j++) {
                    Circle planet = planets[j];
                    double dx = xc - planet.getX();
                    double dy = yc - planet.getY();
                    double distance_squared = dx * dx + dy * dy;
                    double radius_squared = planet.getRadius() * planet.getRadius();
                    if (distance_squared <= radius_squared) {
                        is_blocked = true; // Ray is blocked by a planet
                        break; // No need to check other planets
                    }
                }
            }
        }
    }
}