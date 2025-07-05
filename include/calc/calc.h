#ifdef _WIN32
    #ifdef CALC_BUILDING_DLL
        #define CALC_API __declspec(dllexport)
    #elif defined(CALC_USING_DLL)
        #define CALC_API __declspec(dllimport)
    #else
        #define CALC_API
    #endif
#else
    #define CALC_API
#endif

namespace calc {
    CALC_API double add(double a, double b);
    CALC_API double subtract(double a, double b);
    CALC_API double multiply(double a, double b);
    CALC_API double divide(double a, double b);
    CALC_API double power(double base, double exponent);
    CALC_API double squareRoot(double value);
    CALC_API void version(void);
}