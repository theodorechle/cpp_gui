#ifndef COLOR_CONVERTER_HPP
#define COLOR_CONVERTER_HPP

#include "../style/style_component.hpp"
#include "number_converter.hpp"
#include <SDL3/SDL_pixels.h>
#include <string>
#include <tuple>

namespace gui {
    namespace converter {

        class ColorConverter {
            static bool convertFromHex(style::StyleValue *value, SDL_Color *color);
            static bool convertFromTuple(style::StyleValue *value, SDL_Color *color);

        public:
            /**
             * Returns true in case of successful conversion, false else.
             * If not converted, color is not changed.
             */
            static bool convert(style::StyleValue *value, SDL_Color *color);
        };

    } // namespace converter
} // namespace gui

#endif // COLOR_CONVERTER_HPP
