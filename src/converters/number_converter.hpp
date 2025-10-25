#ifndef NUMBER_CONVERTER_HPP
#define NUMBER_CONVERTER_HPP

#include "../../cpp_style/src/style/style_component.hpp"
#include <string>

namespace gui {
    namespace converter {

        class NumberConverter {

        public:
            /**
             * Returns true in case of successful conversion, false else.
             * If not converted, size is not changed.
             */
            static bool convertToInt(style::StyleValue *value, int *result);
        };

    } // namespace converter
} // namespace gui

#endif // NUMBER_CONVERTER_HPP
