#ifndef COLOR_CONVERTER_HPP
#define COLOR_CONVERTER_HPP

#include "../style/style_component.hpp"
#include <SDL3/SDL_pixels.h>
#include <string>
#include <tuple>

class ColorConverter {
    static bool convertFromHex(const StyleValue *value, SDL_Color *color);
    static bool convertFromTuple(const StyleValue *value, SDL_Color *color);

public:
    /**
     * Returns true in case of successfull conversion, false else
     */
    static bool convert(const StyleValue *value, SDL_Color *color);
};

#endif // COLOR_CONVERTER_HPP
