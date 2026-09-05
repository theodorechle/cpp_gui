#include "bool_converter.hpp"

namespace gui {
    namespace converter {
        bool BoolConverter::convert(style::StyleValue *value, bool *boolean) {
            if (value == nullptr || value->type() != style::ValueType::Bool) return false;
            (*boolean) = (value->value() == "true");
            return true;
        }

    } // namespace converter
} // namespace gui
