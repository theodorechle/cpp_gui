#include "node_to_style_component_list.hpp"

StyleComponentType tokenTypeToStyleComponentType(Token token) {
    switch (token) {
    case Token::ElementName:
        return StyleComponentType::ElementName;
    case Token::Class:
        return StyleComponentType::Class;
    case Token::Modifier:
        return StyleComponentType::Modifier;
    case Token::Identifier:
        return StyleComponentType::Identifier;
    default:
        return StyleComponentType::Null;
    }
}

StyleValueType tokenTypeToStyleValueType(Token token) {
    switch (token) {
    case Token::Int:
        return StyleValueType::Int;
    case Token::Float:
        return StyleValueType::Float;
    case Token::Bool:
        return StyleValueType::Bool;
    case Token::String:
        return StyleValueType::String;
    case Token::Tuple:
        return StyleValueType::Tuple;
    case Token::Function:
        return StyleValueType::Function;
    case Token::Unit:
        return StyleValueType::Unit;
    default:
        return StyleValueType::Null;
    }
}

std::list<StyleComponentDataList *> *NodeToStyleComponentList::convertStyleComponents() {
    std::list<StyleComponentDataList *> *styleComponentsLists;
    StyleComponentDataList *requiredStyleComponents;
    Node *declaration;
    Node *componentDeclaration;
    StyleComponentType styleComponentType;

    if (tree == nullptr || tree->getTokenType() != Token::BlockDeclaration) return nullptr;

    styleComponentsLists = new std::list<StyleComponentDataList *>();
    declaration = tree->getChild();
    while (declaration != nullptr && declaration->getTokenType() == Token::Declaration) {
        requiredStyleComponents = new StyleComponentDataList();
        componentDeclaration = declaration->getChild();
        while (componentDeclaration != nullptr) {
            styleComponentType = tokenTypeToStyleComponentType(componentDeclaration->getTokenType());
            if (styleComponentType == StyleComponentType::Null) return styleComponentsLists;

            requiredStyleComponents->push_back(StyleComponentData(componentDeclaration->getValue(), styleComponentType, StyleRelation::SameElement));
            componentDeclaration = componentDeclaration->getNext();
        }
        styleComponentsLists->push_back(requiredStyleComponents);
    }
    return styleComponentsLists;
}

AppliedStyleMap *NodeToStyleComponentList::convertAppliedStyle() {
    AppliedStyleMap *appliedStyle;
    if (tree == nullptr || tree->getTokenType() != Token::BlockDefinition) return nullptr;
    appliedStyle = new AppliedStyleMap();
    return appliedStyle;
}

std::list<StyleComponent *> *NodeToStyleComponentList::createStyleComponents(std::list<std::list<StyleComponentDataList *> *>::const_iterator componentsListIt, StyleComponentDataList *components, AppliedStyleMap *appliedStyle) {
    std::list<StyleComponent *> *styleComponentList;
    StyleComponentDataList::const_iterator componentsIt;
    if (components == nullptr) return nullptr;
    bool islistHead = std::next(componentsListIt) == requiredStyleComponentsLists.cend();
    if (islistHead) {
        styleComponentList = new std::list<StyleComponent *>();
        for (StyleComponentDataList *components : **componentsListIt) {
            StyleComponent *styleComponent = new StyleComponent(std::get<0>(components->back()), std::get<1>(components->back()));

            componentsIt = std::prev(components->end());
            components->splice(components->end(), *components);

            styleComponent->addStyleDefinition(components, appliedStyle);
            styleComponentList->push_back(styleComponent);

            components->erase(componentsIt);
        }
    } else {
        for (StyleComponentDataList *componentsList : **componentsListIt) {

            componentsIt = std::prev(components->end());
            components->splice(components->end(), *componentsList);

            styleComponentList = createStyleComponents(std::next(componentsListIt), components, appliedStyle);

            components->erase(componentsIt);
        }
    }
    return styleComponentList;
}

std::list<StyleComponent *> *NodeToStyleComponentList::convertStyleBlock() {
    if (tree == nullptr || tree->getTokenType() != Token::StyleBlock) return nullptr;
    tree = tree->getChild();
    std::list<StyleComponentDataList *> *styleComponentsLists = convertStyleComponents();
    if (styleComponentsLists == nullptr || styleComponentsLists->empty()) return nullptr;

    requiredStyleComponentsLists.push_back(styleComponentsLists);
    tree = tree->getNext();

    AppliedStyleMap *appliedStyle = convertAppliedStyle();
    tree = tree->getParent();
    if (appliedStyle == nullptr || appliedStyle->empty()) return nullptr;

    StyleComponentDataList components = StyleComponentDataList();
    return createStyleComponents(requiredStyleComponentsLists.cbegin(), &components, appliedStyle);
}

std::list<StyleComponent *> *NodeToStyleComponentList::convert(Node *styleTree) {
    std::list<StyleComponent *> *styleDefinitions = new std::list<StyleComponent *>();
    ;
    std::list<StyleComponent *> *convertedStyleBlock;
    requiredStyleComponentsLists = std::list<std::list<StyleComponentDataList *> *>();

    if (styleTree->getTokenType() != Token::NullRoot) return styleDefinitions;

    tree = styleTree->getChild();
    while (tree != nullptr && tree->getTokenType() == Token::StyleBlock) {
        convertedStyleBlock = convertStyleBlock();
        styleDefinitions->splice(styleDefinitions->end(), *convertedStyleBlock);
        tree = tree->getNext();
    }

    return styleDefinitions;
}