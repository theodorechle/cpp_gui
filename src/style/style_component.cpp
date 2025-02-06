#include "style_component.hpp"

namespace style {

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
        case StyleValueType::Hex:
            return "Hex";
        case StyleValueType::NameString:
            return "NameString";
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

    int StyleValue::nbChilds() const {
        int nb = 0;
        StyleValue *currentChild = this->child;
        while (currentChild != nullptr) {
            nb++;
            currentChild = currentChild->next;
        }
        return nb;
    }

    StyleValue::~StyleValue() {
        delete child;
        delete next;
    }

    StyleBlock::StyleBlock(StyleComponentDataList *componentsList,
                           StyleValuesMap *styleMap) { // TODO: forbid null pointers (maybe get params as references ?)
        styleDef = new StyleDefinition(*componentsList, *styleMap);
    }

    StyleBlock::~StyleBlock() {
        for (std::pair<std::string, StyleRule> rule : styleDef->second) {
            delete rule.second.value;
        }
        delete styleDef;
    }

} // namespace Style
