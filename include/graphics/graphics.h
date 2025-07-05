#include <SDL3/SDL_surface.h>

#define RAY_COUNT 360

namespace graphics {
    struct Circle {
        double x, y, r;
        Circle(double x, double y, double r) : x(x), y(y), r(r) {}
    };
    struct Ray {
        // angle in radians
        double x, y, a;
        Ray(double x, double y, double angle) : x(x), y(y), a(a) {}
    };
    void drawCircle(SDL_Surface* surface, graphics::Circle circle, Uint32 color);
    void generateSunrays(Circle sun, struct Ray rays[RAY_COUNT]);
    void drawSunrays(SDL_Surface* surface, graphics::Circle sun, struct Ray rays[RAY_COUNT]);
}