#include<stdio.h>
#include<math.h>
#include<SDL3/SDL.h>
#include "graphics/graphics.h"
// https://www.youtube.com/watch?v=2BLRLuczykM
namespace graphics {
    void drawCircle(SDL_Surface* surface, Circle circle, Uint32 color) {
        double xc = circle.x;
        double yc = circle.y;
        double r = circle.r;
        double rsq = pow(r, 2);
        double x_end = xc + r;
        double y_end = yc + r;
        for (double x = xc - r; x <= x_end; x++) {
            for (double y = yc - r; y <= y_end; y++) {
                if (pow(x - xc, 2) + pow(y - yc, 2) <= rsq) {
                    SDL_Rect pixel = (SDL_Rect) {(int)x, (int)y, 1, 1};
                    SDL_FillSurfaceRect(surface, &pixel, color);
                }
            }
        }
    }

    void generateSunrays(Circle sun, Ray rays[RAY_COUNT]) {
        double angle_step = 360 / RAY_COUNT;
        for (int i = 0; i < RAY_COUNT; i++) {
            double angle = i * angle_step * (M_PI / 180.0); // Convert degrees to radians
            rays[i] = Ray(sun.x, sun.y, angle);
        }
    }

    void drawSunrays(SDL_Surface* surface, Circle sun, Ray rays[RAY_COUNT]) {
        // Yellow color
        Uint32 color = 0xffff00ff; // ARGB format: 0xAARRGGBB
        double xc = sun.x;
        double yc = sun.y;
        bool is_outside_window = false;
        bool is_blocked = false;
        for (int i = 0; i < RAY_COUNT; i++) {
            Ray* rays = &rays[i];
            while (rays->a < 360) {
                double angle = rays->a; // Convert angle to radians
                xc += cos(angle) * sun.r; // Calculate end point of the ray
                yc += sin(angle) * sun.r; // Calculate end point of the ray
                rays++;

                if (is_outside_window || is_blocked) {
                    break;
                }
            }
        }
    }
}