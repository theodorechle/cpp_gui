#include "bool_converter.hpp"

namespace gui {
    namespace converter {
        bool BoolConverter::convert(style::StyleValue *value, bool *boolean) {
            if (value == nullptr || value->getType() != style::StyleValueType::Bool) return false;
            (*boolean) = (value->getValue() == "true");
            return true;
        }

    } // namespace converter
} // namespace gui
