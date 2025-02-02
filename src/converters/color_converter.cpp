#include "color_converter.hpp"

namespace gui {
    namespace converter {

        bool ColorConverter::convertFromHex(style::StyleValue *value, SDL_Color *color) {
            std::string valueStr = value->getValue();
            switch (valueStr.size()) {
            case 3:
                color->r = std::stoul(valueStr.substr(0, 1) + valueStr.substr(0, 1), nullptr, 16);
                color->g = std::stoul(valueStr.substr(1, 1) + valueStr.substr(1, 1), nullptr, 16);
                color->b = std::stoul(valueStr.substr(2, 1) + valueStr.substr(2, 1), nullptr, 16);
                color->a = 255;
                return true;
            case 6:
                color->r = std::stoul(valueStr.substr(0, 2), nullptr, 16);
                color->g = std::stoul(valueStr.substr(2, 2), nullptr, 16);
                color->b = std::stoul(valueStr.substr(4, 2), nullptr, 16);
                color->a = 255;
                return true;
            case 8:
                color->r = std::stoul(valueStr.substr(0, 2), nullptr, 16);
                color->g = std::stoul(valueStr.substr(2, 2), nullptr, 16);
                color->b = std::stoul(valueStr.substr(4, 2), nullptr, 16);
                color->a = std::stoul(valueStr.substr(6, 2), nullptr, 16);
                return true;
            default:
                return false;
            }
        }

        bool ColorConverter::convertFromTuple(style::StyleValue *value, SDL_Color *color) {
            style::StyleValue *child = value->getChild();
            int intResult;
            switch (value->nbChilds()) {
            case 3:
                if (!NumberConverter::convertToInt(child, &intResult)) return false;
                color->r = intResult;
                child = child->getNext();
                if (!NumberConverter::convertToInt(child, &intResult)) return false;
                color->g = intResult;
                child = child->getNext();
                if (!NumberConverter::convertToInt(child, &intResult)) return false;
                color->b = intResult;
                color->a = 255;
                return true;
            case 4:
                if (!NumberConverter::convertToInt(child, &intResult)) return false;
                color->r = intResult;
                child = child->getNext();
                if (!NumberConverter::convertToInt(child, &intResult)) return false;
                color->g = intResult;
                child = child->getNext();
                if (!NumberConverter::convertToInt(child, &intResult)) return false;
                color->b = intResult;
                child = child->getNext();
                if (!NumberConverter::convertToInt(child, &intResult)) return false;
                color->a = intResult;
                return true;
            default:
                return false;
            }
        }

        bool ColorConverter::convert(style::StyleValue *value, SDL_Color *color) {
            if (value == nullptr || color == nullptr) return false;
            switch (value->getType()) {
            case style::StyleValueType::Hex:
                return convertFromHex(value, color);
            case style::StyleValueType::Tuple:
                return convertFromTuple(value, color);
            default:
                return false;
            }
        }

    } // namespace converter
} // namespace gui
