#include<math.h>
#include<SDL3/SDL.h>
#include "graphics/graphics.h"

namespace graphics {
    void drawCircle(SDL_Surface* surface, Circle circle, Uint32 color) {
        double xc = circle.x;
        double yc = circle.y;
        double r = circle.r;
        double rsq = pow(r, 2);
        double x = xc - r;
        double y = yc - r;
        double x_end = xc + r;
        double y_end = yc + r;
        for (; x <= x_end; x++) {
            for (; y <= y_end; y++) {
                if (pow(x, 2) + pow(y, 2) <= rsq) {
                    SDL_Rect pixel = (SDL_Rect) {(int)x, (int)y, 1, 1};
                    SDL_FillSurfaceRect(surface, &pixel, color);
                }
            }
        }
    }
}