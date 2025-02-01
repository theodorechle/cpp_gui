#ifndef STYLE_COMPONENT_HPP
#define STYLE_COMPONENT_HPP

#include "tokens.hpp"

#include <list>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace style {

    enum class StyleComponentType { ElementName, Class, Modifier, Identifier, Null };

    enum class StyleValueType { Int, Float, Bool, String, Tuple, Function, PercentageUnit, PixelUnit, Hex, None, Auto, Null };

    enum class StyleRelation { SameElement, DirectParent, AnyParent, Null };

    StyleComponentType tokenTypeToStyleComponentType(Token token);
    StyleValueType tokenTypeToStyleValueType(Token token);
    StyleRelation tokenTypeToStyleRelation(Token token);

    std::string styleComponentTypeToString(StyleComponentType token);
    std::string styleValueTypeToString(StyleValueType token);
    std::string styleRelationToString(StyleRelation token);

    /**
     * Some data types are containing others, for example a tuple contains multiple elements.
     * This class allows such elements who contains others to exists.
     */
    class StyleValue {
        std::string value;
        StyleValueType type;
        StyleValue *child = nullptr;
        StyleValue *next = nullptr;

    public:
        StyleValue(const std::string &value = "", const StyleValueType type = StyleValueType::Null) : value{value}, type{type} {};
        void setValue(const std::string &value) { this->value = value; }
        void setType(StyleValueType type) { this->type = type; }
        void setChild(StyleValue *child) { this->child = child; }
        void setNext(StyleValue *next) { this->next = next; }
        std::string getValue() const { return value; }
        StyleValueType getType() const { return type; }
        StyleValue *getChild() { return child; }
        StyleValue *getNext() { return next; }
        ~StyleValue();
    };

    /**
     *   style value
     *   enabled
     *   specificity
     *   file number
     *   rule number (file dependant)
     */
    struct StyleRule {
        StyleValue *value;
        bool enabled;
        int specificity;
        int fileNumber;
        int ruleNumber;
    };

    typedef std::pair<std::string, StyleComponentType> StyleComponentData;
    typedef std::list<std::pair<StyleComponentData, StyleRelation>> StyleComponentDataList;
    typedef std::unordered_map<std::string, StyleRule> StyleValuesMap;
    typedef std::pair<StyleComponentDataList, StyleValuesMap> StyleDefinition;

    /**
     * A block composed of a list of required components (parent classes, special identifier, ...) and the corresponding style
     */
    class StyleBlock {
        StyleDefinition *styleDef;

    public:
        StyleBlock(StyleComponentDataList *componentsList, StyleValuesMap *styleMap);
        ~StyleBlock();
        StyleDefinition *getStyleDefinition() const { return styleDef; }
        const StyleComponentDataList *getComponentsList() const { return &styleDef->first; }
        const StyleValuesMap *getStyleMap() const { return &styleDef->second; }
    };

} // namespace style

#endif // STYLE_COMPONENT_HPP
