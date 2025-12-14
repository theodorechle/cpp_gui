#include "number_converter.hpp"

namespace gui {
    namespace converter {
        bool NumberConverter::convertToInt(style::StyleValue *value, int *result) {
            std::string valueStr = value->value();
            if (value->type() == style::StyleValueType::Int) {
                (*result) = std::stoul(valueStr, nullptr, 10);
                return true;
            }
            return false;
        }
    } // namespace converter
} // namespace gui
