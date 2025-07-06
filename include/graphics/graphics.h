#include <SDL3/SDL_surface.h>
#include <cmath>

#ifdef _WIN32
    #ifdef GRAPHICS_BUILDING_DLL
        #define GRAPHICS_API __declspec(dllexport)
    #elif defined(GRAPHICS_USING_DLL)
        #define GRAPHICS_API __declspec(dllimport)
    #else
        #define GRAPHICS_API
    #endif
#else
    #define GRAPHICS_API
#endif

#define RAY_COUNT 100

namespace graphics {
    struct Circle {
        double x, y, r;
        Circle(double x, double y, double r) : x(x), y(y), r(r) {}
    };
    struct Ray {
        // angle in radians
        double x, y, a;
    };
    GRAPHICS_API void drawCircle(SDL_Surface* surface, struct graphics::Circle circle, Uint32 color);
    GRAPHICS_API void generateRays(struct graphics::Circle sun, struct Ray rays[RAY_COUNT]);
    GRAPHICS_API void drawSunrays(SDL_Surface* surface, struct graphics::Circle sun, struct Ray rays[RAY_COUNT], Uint32 color);
}