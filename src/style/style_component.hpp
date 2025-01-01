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

/**
 * All data types are not simple.
 * For example a tuple contains multiple elements.
 * That's why this class exists, to allow such elements who contains others to exists.
 */
class StyleValue {
    std::string styleName;
    StyleValueType styleType;
    StyleValue *styleChild;
    StyleValue *styleNext;

public:
    void setName(const std::string &name) { styleName = name; }
    void setType(StyleValueType type) { styleType = type; }
    void setChild(StyleValue *child) { styleChild = child; }
    void setNext(StyleValue *next) { styleNext = next; }
    std::string getName() { return styleName; }
    StyleValueType getType() { return styleType; }
    StyleValue *getChild() { return styleChild; }
    StyleValue *getNext() { return styleNext; }
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

class StyleComponent {
    StyleDefinition *styleDef;

public:
    StyleComponent(StyleComponentDataList *componentsList, StyleValuesMap *styleMap);
    ~StyleComponent() {delete styleDef;}
    StyleDefinition *getStyleDefinition() const { return styleDef; }
    const StyleComponentDataList *getComponentsList() const { return &styleDef->first; }
    const StyleValuesMap *getStyleMap() const { return &styleDef->second; }
};

#endif // STYLE_COMPONENT_HPP
