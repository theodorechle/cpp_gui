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
    Unit,
    Null
};

enum class StyleRelation {
    SameElement,
    DirectParent,
    AnyParent
};

StyleComponentType tokenTypeToStyleComponentType(Token token);
StyleValueType tokenTypeToStyleValueType(Token token);

typedef std::tuple<std::string, StyleComponentType, StyleRelation> StyleComponentData;
typedef std::list<StyleComponentData> StyleComponentDataList;
typedef std::unordered_map<std::string, std::tuple<std::string, StyleValueType>> AppliedStyleMap;
typedef std::list<std::pair<StyleComponentDataList, AppliedStyleMap>> StyleDefinition;

class StyleComponent {
    std::string name;
    StyleComponentType type;
    StyleDefinition *styleDef;

public:
    StyleComponent(const std::string &name, StyleComponentType type) : name{name}, type{type} {}
    std::string getName() const { return name; }
    StyleComponentType getType() const { return type; }
    StyleDefinition *getStyleDefinition() const { return styleDef; }
    void addStyleDefinition(StyleComponentDataList *componentsList, AppliedStyleMap *styleMap);
};

#endif // STYLE_COMPONENT_HPP
