#ifndef STYLE_COMPONENT_HPP
#define STYLE_COMPONENT_HPP

#include <string>
#include <list>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

enum class StyleComponentType {
    ElementName,
    Class,
    Modifier,
    Identifier
};

enum class StyleValueType {
    Int,
    Float,
    Bool,
    String,
    Tuple,
    Function,
    Unit
};

enum class StyleRelation {
    DirectParent,
    AnyParent
};

typedef std::vector<std::tuple<std::string, StyleComponentType, StyleRelation>> RequiredStyleComponentsList;
typedef std::unordered_map<std::string, std::tuple<std::string, StyleValueType>> AppliedStyleMap;
typedef std::list<std::pair<RequiredStyleComponentsList, AppliedStyleMap>> StyleDefinition;

class StyleComponent {
    std::string name;
    StyleComponentType type;
    StyleDefinition styleDef;
public:
    std::string getName() const {return name;}
    StyleComponentType getType() const {return type;}
    void addStyleDefinition(RequiredStyleComponentsList componentsList, AppliedStyleMap styleMap);
    const StyleDefinition& getStyleDefinition() const;
};

#endif // STYLE_COMPONENT_HPP
