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
            std::string valueStr = value->getChild()->getValue();
            if (value->getChild()->getType() == style::StyleValueType::Int) {
                (*size) = std::stoul(valueStr, nullptr, 10);
                (*size) = ((*size) * parentSize / 100);
                return true;
            }
            return false;
        }

        bool gui::converter::SizeConverter::convert(style::StyleValue *value, int *size, int parentSize) {
            if (value == nullptr || size == nullptr) return false;
            switch (value->getType()) {
            case style::StyleValueType::PixelUnit:
                return convertFromPixel(value, size);
            case style::StyleValueType::PercentageUnit:
                return convertFromPercentage(value, size, parentSize);
            default:
                return false;
            }
        }

    } // namespace converter
} // namespace gui
