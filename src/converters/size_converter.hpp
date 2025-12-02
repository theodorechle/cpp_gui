#ifndef SIZE_CONVERTER_HPP
#define SIZE_CONVERTER_HPP

#include "../../cpp_style/src/style_component.hpp"
#include "number_converter.hpp"
#include <string>

namespace gui {
    namespace converter {

        class SizeConverter {
            static bool convertFromPixel(style::StyleValue *value, int *size);
            static bool convertFromPercentage(style::StyleValue *value, int *size, int parentSize);

        public:
            /**
             * Returns true in case of successful conversion, false else.
             * If not converted, size is not changed.
             * If relative size (%) and rellativeSize is not nullptr, sets relativeSize to true. Does not set relativeSize to false at any time
             */
            static bool convert(style::StyleValue *value, int *size, int parentSize);
        };

    } // namespace converter
} // namespace gui

#endif // SIZE_CONVERTER_HPP
