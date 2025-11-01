#include "size_converter.hpp"

namespace gui {
    namespace converter {

        bool gui::converter::SizeConverter::convertFromPixel(style::StyleValue *value, int *size) {
            std::string valueStr = value->getChild()->getValue();
            if (value->getChild()->getType() == style::StyleValueType::Int) {
                *size = std::stoul(valueStr, nullptr, 10);
                return true;
            }
            return false;
        }

        bool gui::converter::SizeConverter::convertFromPercentage(style::StyleValue *value, int *size, int parentSize) {
            // (*size) = 0;
            // return false; // FIXME: don't work because parent's size is null on first iteration
            if (!NumberConverter::convertToInt(value->getChild(), size)) return false;
            (*size) = ((*size) * parentSize / 100);
            return true;
        }

        bool gui::converter::SizeConverter::convert(style::StyleValue *value, int *size, int parentSize) {
            if (value == nullptr || size == nullptr) return false;
            std::string v = value->getValue();
            if (v == "px") return convertFromPixel(value, size);
            if (v == "%") return convertFromPercentage(value, size, parentSize);
            return false;
        }

    } // namespace converter
} // namespace gui
