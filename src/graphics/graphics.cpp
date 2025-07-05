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
        for (double x = xc - r; x <= x_end; x++) {
            for (double y = yc - r; y <= y_end; y++) {
                if (pow(x - xc, 2) + pow(y - yc, 2) <= rsq) {
                    SDL_Rect pixel = {(int)x, (int)y, 1, 1};
                    SDL_FillSurfaceRect(surface, &pixel, color);
                }
            }
        }
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

    void drawSunrays(SDL_Surface* surface, struct Circle sun, struct Ray rays[RAY_COUNT]) {
        // Yellow color
        Uint32 color = 0xffff00ff; // ARGB format: 0xAARRGGBB
        double xc = sun.x;
        double yc = sun.y;
        bool is_outside_window = false;
        bool is_blocked = false;
        for (int i = 0; i < RAY_COUNT; i++) {
            Ray* ray = &rays[i];
            // while (ray->a < 360) {
            //     double angle = ray->a; // Convert angle to radians
            //     xc += cos(angle) * sun.r; // Calculate end point of the ray
            //     yc += sin(angle) * sun.r; // Calculate end point of the ray
            //     ray++;

            //     if (is_outside_window || is_blocked) {
            //         break;
            //     }
            // }
        }
    }
}