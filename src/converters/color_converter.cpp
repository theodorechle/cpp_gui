#include "color_converter.hpp"

bool ColorConverter::convertFromHex(const StyleValue *value, SDL_Color *color) {
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

bool ColorConverter::convertFromTuple(const StyleValue *value, SDL_Color *color) {
    return false;
}

bool ColorConverter::convert(const StyleValue *value, SDL_Color *color) {
    if (value == nullptr || color == nullptr) return false;
    switch (value->getType()) {
    case StyleValueType::Hex:
        return convertFromHex(value, color);
    case StyleValueType::Tuple:
        return convertFromTuple(value, color);
    default:
        return false;
    }
}