#ifndef BOOL_CONVERTER_HPP
#define BOOL_CONVERTER_HPP

#include "../../cpp_style/src/style_component.hpp"

namespace gui {
    namespace converter {

        class BoolConverter {
            public:
            static bool convert(style::StyleValue *value, bool *boolean);
        };

    } // namespace converter
} // namespace gui

#endif // BOOL_CONVERTER_HPP
