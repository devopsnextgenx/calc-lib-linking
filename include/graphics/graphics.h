#include <SDL3/SDL_surface.h>
namespace graphics {
    struct Circle {
        double x, y, r;
        Circle(double x, double y, double r) : x(x), y(y), r(r) {}
    };

    void drawCircle(SDL_Surface* surface, graphics::Circle circle, Uint32 color);
}