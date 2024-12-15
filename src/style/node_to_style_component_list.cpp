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
    case Token::PercentageUnit:
        return StyleValueType::PercentageUnit;
    case Token::PixelUnit:
        return StyleValueType::PixelUnit;
    default:
        return StyleValueType::Null;
    }
}

AppliedStyle::~AppliedStyle() {
    delete styleChild;
    delete styleNext;
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
    // loop through declarations
    while (declaration != nullptr && declaration->getTokenType() == Token::Declaration) {
        requiredStyleComponents = new StyleComponentDataList();
        componentDeclaration = declaration->getChild();
        // loop through each value in a declaration
        while (componentDeclaration != nullptr) {
            styleComponentType = tokenTypeToStyleComponentType(componentDeclaration->getTokenType());
            if (styleComponentType == StyleComponentType::Null) { // if invalid declaration, go to next one
                componentDeclaration = componentDeclaration->getNext();
                continue;
            }
            // TODO : change DirectElement to true value
            requiredStyleComponents->push_back(StyleComponentData(componentDeclaration->getValue(), styleComponentType, StyleRelation::SameElement));
            componentDeclaration = componentDeclaration->getNext();
        }
        styleComponentsLists->push_back(requiredStyleComponents);
        declaration = declaration->getNext();
    }
    return styleComponentsLists;
}

AppliedStyle *NodeToStyleComponentList::convertStyleNodesToAppliedStyle(Node *node) {
    StyleValueType type;
    Node *child;
    AppliedStyle *styleChild;
    AppliedStyle *appliedStyle;

    type = tokenTypeToStyleValueType(node->getTokenType());
    if (type == StyleValueType::Null) return nullptr;

    appliedStyle = new AppliedStyle();
    appliedStyle->setName(node->getValue());
    appliedStyle->setType(type);

    child = node->getChild();
    if (child == nullptr) return appliedStyle;
    styleChild = convertStyleNodesToAppliedStyle(child);
    if (styleChild != nullptr) {
        appliedStyle->setChild(styleChild);
        appliedStyle = styleChild;
    }

    child = child->getNext();
    while (child != nullptr) {
        styleChild = convertStyleNodesToAppliedStyle(child);
        if (styleChild != nullptr) {
            appliedStyle->setNext(styleChild);
            appliedStyle = styleChild;
        }
        child = child->getNext();
    }
    return appliedStyle;
}

AppliedStyleMap *NodeToStyleComponentList::convertAppliedStyle() {
    AppliedStyleMap *appliedStyleMap;
    AppliedStyle *appliedStyle;
    std::string styleName;
    Node *definition;
    Node *value;
    Token token;
    if (tree == nullptr || tree->getTokenType() != Token::BlockDefinition) return nullptr;

    appliedStyleMap = new AppliedStyleMap();
    definition = tree->getChild();
    while (definition != nullptr) {
        token = definition->getTokenType();
        if (token == Token::Assignment) {
            value = definition->getChild();
            if (value == nullptr || value->getTokenType() != Token::StyleName) {
                definition = definition->getNext();
                continue;
            }
            styleName = definition->getValue();
            value = value->getNext();
            if (!isNodeNull(value)) {
                appliedStyle = convertStyleNodesToAppliedStyle(value);
                if (appliedStyle != nullptr) (*appliedStyleMap)[styleName] = appliedStyle;
            }
        } else if (token == Token::StyleBlock) {
            convertStyleBlock();
        }
        // if other token type, just don't use the definition
        definition = definition->getNext();
    }

    return appliedStyleMap;
}

std::list<StyleComponent *> *NodeToStyleComponentList::createStyleComponents(std::list<std::list<StyleComponentDataList *> *>::const_iterator componentsListIt, StyleComponentDataList *components, AppliedStyleMap *appliedStyleMap) {
    std::list<StyleComponent *> *styleComponentList = new std::list<StyleComponent *>();
    std::list<StyleComponent *> *tmpStyleComponentList;
    StyleComponentDataList::const_iterator componentsIt;
    if (components == nullptr) return nullptr;
    bool islistHead = std::next(componentsListIt) == requiredStyleComponentsLists.cend();
    if (islistHead) { // if at end of the declaration list
        for (StyleComponentDataList *components : **componentsListIt) {
            StyleComponent *styleComponent = new StyleComponent(std::get<0>(components->back()), std::get<1>(components->back()));

            componentsIt = std::prev(components->end());
            components->splice(components->end(), *components);

            styleComponent->addStyleDefinition(components, appliedStyleMap);
            styleComponentList->push_back(styleComponent);

            components->erase(componentsIt);
        }
    } else {
        for (StyleComponentDataList *componentsList : **componentsListIt) {
            componentsIt = std::prev(components->end());
            components->splice(components->end(), *componentsList);

            tmpStyleComponentList = createStyleComponents(std::next(componentsListIt), components, appliedStyleMap);
            styleComponentList->splice(styleComponentList->end(), *tmpStyleComponentList);
            delete tmpStyleComponentList;

            components->erase(componentsIt);
        }
    }
    return styleComponentList;
}

void NodeToStyleComponentList::convertStyleBlock() {
    std::list<StyleComponentDataList *> *styleComponentsLists;
    std::list<StyleComponent *> *finalStyleComponents;
    if (tree == nullptr || tree->getTokenType() != Token::StyleBlock) return;
    tree = tree->getChild();
    styleComponentsLists = convertStyleComponents();
    if (styleComponentsLists == nullptr || styleComponentsLists->empty()) {
        delete styleComponentsLists;
        return;
    }
    tree = tree->getNext();

    AppliedStyleMap *appliedStyleMap = convertAppliedStyle();
    tree = tree->getParent();
    if (appliedStyleMap == nullptr || appliedStyleMap->empty()) {
        delete styleComponentsLists;
        delete appliedStyleMap;
        return;
    }
    requiredStyleComponentsLists.push_back(styleComponentsLists);
    StyleComponentDataList components = StyleComponentDataList();
    finalStyleComponents = createStyleComponents(requiredStyleComponentsLists.cbegin(), &components, appliedStyleMap);
    requiredStyleComponentsLists.pop_back();
    styleDefinitions->splice(styleDefinitions->end(), *finalStyleComponents);
}

std::list<StyleComponent *> *NodeToStyleComponentList::convert(Node *styleTree) {
    styleDefinitions = new std::list<StyleComponent *>();
    requiredStyleComponentsLists = std::list<std::list<StyleComponentDataList *> *>();

    if (styleTree->getTokenType() != Token::NullRoot) return styleDefinitions;

    tree = styleTree->getChild();
    while (tree != nullptr && tree->getTokenType() == Token::StyleBlock) {
        convertStyleBlock();
        tree = tree->getNext();
    }

    return styleDefinitions;
}