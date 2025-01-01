#include "style_component.hpp"

std::string styleComponentTypeToString(StyleComponentType token) {
    switch (token) {
    case StyleComponentType::ElementName:
        return "ElementName";
    case StyleComponentType::Class:
        return "Class";
    case StyleComponentType::Modifier:
        return "Modifier";
    case StyleComponentType::Identifier:
        return "Identifier";
    case StyleComponentType::Null:
        return "Null";
    default:
        return "Unknown";
    }
}

std::string styleValueTypeToString(StyleValueType token) {
    switch (token) {
    case StyleValueType::Int:
        return "Int";
    case StyleValueType::Float:
        return "Float";
    case StyleValueType::Bool:
        return "Bool";
    case StyleValueType::String:
        return "String";
    case StyleValueType::Tuple:
        return "Tuple";
    case StyleValueType::Function:
        return "Function";
    case StyleValueType::PercentageUnit:
        return "PercentageUnit";
    case StyleValueType::PixelUnit:
        return "PixelUnit";
    case StyleValueType::Null:
        return "Null";
    default:
        return "Unknown";
    }
}

std::string styleRelationToString(StyleRelation token) {
    switch (token) {
    case StyleRelation::SameElement:
        return "SameElement";
    case StyleRelation::DirectParent:
        return "DirectParent";
    case StyleRelation::AnyParent:
        return "AnyParent";
    case StyleRelation::Null:
        return "Null";
    default:
        return "Unknown";
    }
}

StyleBlock::StyleBlock(StyleComponentDataList *componentsList, StyleValuesMap *styleMap) {
    styleDef = new StyleDefinition(*componentsList, *styleMap);
}
