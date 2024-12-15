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
    AnyParent
};

StyleComponentType tokenTypeToStyleComponentType(Token token);
StyleValueType tokenTypeToStyleValueType(Token token);

class AppliedStyle {
    std::string styleName;
    StyleValueType styleType;
    AppliedStyle *styleChild;
    AppliedStyle *styleNext;
public:
    void setName(const std::string &name) {styleName = name;}
    void setType(StyleValueType type) {styleType = type;}
    void setChild(AppliedStyle *child) {styleChild = child;}
    void setNext(AppliedStyle *next) {styleNext = next;}
    std::string getName() {return styleName;}
    StyleValueType getType() {return styleType;}
    AppliedStyle *getChild() {return styleChild;}
    AppliedStyle *getNext() {return styleNext;}
    ~AppliedStyle();
};

typedef std::tuple<std::string, StyleComponentType, StyleRelation> StyleComponentData;
typedef std::list<StyleComponentData> StyleComponentDataList;
typedef std::unordered_map<std::string, AppliedStyle*> AppliedStyleMap;
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
