#ifndef STYLE_COMPONENT_HPP
#define STYLE_COMPONENT_HPP

#include "tokens.hpp"

#include <list>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

enum class StyleComponentType {
    ElementName,
    Class,
    Modifier,
    Identifier,
    Null
};

enum class StyleValueType {
    Int,
    Float,
    Bool,
    String,
    Tuple,
    Function,
    PercentageUnit,
    PixelUnit,
    Null
};

enum class StyleRelation {
    SameElement,
    DirectParent,
    AnyParent,
    Null
};

StyleComponentType tokenTypeToStyleComponentType(Token token);
StyleValueType tokenTypeToStyleValueType(Token token);

std::string styleComponentTypeToString(StyleComponentType token);
std::string styleValueTypeToString(StyleValueType token);
std::string styleRelationToString(StyleRelation token);

/**
 * All data types are not simple.
 * For example a tuple contains multiple elements.
 * That's why this class exists, to allow such elements who contains others to exists.
 */
class StyleValue {
    std::string name;
    StyleValueType type;
    StyleValue *child = nullptr;
    StyleValue *next = nullptr;

public:
    StyleValue(const std::string &name = "", const StyleValueType type = StyleValueType::Null) : name{name}, type{type} {};
    void setName(const std::string &name) { this->name = name; }
    void setType(StyleValueType type) { this->type = type; }
    void setChild(StyleValue *child) { this->child = child; }
    void setNext(StyleValue *next) { this->next = next; }
    std::string getName() { return name; }
    StyleValueType getType() { return type; }
    StyleValue *getChild() { return child; }
    StyleValue *getNext() { return next; }
    ~StyleValue();
};

/**
 *   style value
 *   isEnabled
 *   priority
 *   file number
 *   rule number (file dependant)
 */
struct StyleRule {
    StyleValue *value;
    bool isEnabled;
    int priority;
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
    ~StyleBlock() { delete styleDef; }
    StyleDefinition *getStyleDefinition() const { return styleDef; }
    const StyleComponentDataList *getComponentsList() const { return &styleDef->first; }
    const StyleValuesMap *getStyleMap() const { return &styleDef->second; }
};

#endif // STYLE_COMPONENT_HPP
