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

StyleValue::~StyleValue() {
    delete child;
    delete next;
}

std::list<StyleComponentDataList *> *NodeToStyleComponentList::convertStyleComponents() {
    std::list<StyleComponentDataList *> *styleComponentsLists;
    StyleComponentDataList *requiredStyleComponents;
    Node *declaration;
    Node *componentDeclaration;
    Token nextDeclarationToken;
    StyleComponentType styleComponentType;
    StyleRelation styleRelationToken;
    std::string currentValue;

    if (tree == nullptr || tree->getTokenType() != Token::BlockDeclaration) return nullptr;

    styleComponentsLists = new std::list<StyleComponentDataList *>();
    declaration = tree->getChild();
    // loop through declarations
    while (declaration != nullptr) {
        if (declaration->getTokenType() != Token::Declaration) { // invalid block declaration
            delete styleComponentsLists;
            return nullptr;
        }
        requiredStyleComponents = new StyleComponentDataList();
        componentDeclaration = declaration->getChild();
        // loop through each value in a declaration
        while (componentDeclaration != nullptr) {
            styleComponentType = tokenTypeToStyleComponentType(componentDeclaration->getTokenType());
            currentValue = componentDeclaration->getValue();
            componentDeclaration = componentDeclaration->getNext();

            // if invalid declaration, skip it
            if (styleComponentType == StyleComponentType::Null) continue;

            styleRelationToken = StyleRelation::Null;

            if (componentDeclaration == nullptr) styleRelationToken = StyleRelation::SameElement; // last value
            else {
                nextDeclarationToken = componentDeclaration->getTokenType();
                if (tokenTypeToStyleComponentType(nextDeclarationToken) != StyleComponentType::Null) {
                    styleRelationToken = StyleRelation::SameElement;
                }
                else if (nextDeclarationToken == Token::DirectParent) {
                    styleRelationToken = StyleRelation::DirectParent;
                    componentDeclaration = componentDeclaration->getNext();
                }
                else if (nextDeclarationToken == Token::AnyParent) {
                    styleRelationToken = StyleRelation::AnyParent;
                    componentDeclaration = componentDeclaration->getNext();
                }
            }
            if (styleRelationToken != StyleRelation::Null) {
                requiredStyleComponents->push_back(std::pair(StyleComponentData(currentValue, styleComponentType), styleRelationToken));
            }
        }
        styleComponentsLists->push_back(requiredStyleComponents);
        declaration = declaration->getNext();
    }
    return styleComponentsLists;
}

StyleValue *NodeToStyleComponentList::convertStyleNodeToStyleValue(Node *node) {
    StyleValueType type;
    Node *child;
    StyleValue *styleChild;
    StyleValue *appliedStyle;

    type = tokenTypeToStyleValueType(node->getTokenType());
    if (type == StyleValueType::Null) return nullptr;

    appliedStyle = new StyleValue();
    appliedStyle->setName(node->getValue());
    appliedStyle->setType(type);

    child = node->getChild();
    if (child == nullptr) return appliedStyle;
    styleChild = convertStyleNodeToStyleValue(child);
    if (styleChild != nullptr) {
        appliedStyle->setChild(styleChild);
        appliedStyle = styleChild;
    }

    child = child->getNext();
    while (child != nullptr) {
        styleChild = convertStyleNodeToStyleValue(child);
        if (styleChild != nullptr) {
            appliedStyle->setNext(styleChild);
            appliedStyle = styleChild;
        }
        child = child->getNext();
    }
    return appliedStyle;
}

StyleValuesMap *NodeToStyleComponentList::convertAppliedStyle(int fileNumber, int *ruleNumber) {
    StyleValuesMap *appliedStyleMap;
    StyleValue *styleValue;
    std::string styleName;
    Node *definition;
    Node *styleNameNode;
    Token token;
    if (tree == nullptr || tree->getTokenType() != Token::BlockDefinition) return nullptr;

    appliedStyleMap = new StyleValuesMap();
    definition = tree->getChild();
    while (definition != nullptr) {
        token = definition->getTokenType();
        if (token == Token::Assignment) {
            styleNameNode = definition->getChild();
            if (styleNameNode == nullptr || styleNameNode->getTokenType() != Token::StyleName) {
                definition = definition->getNext();
                continue;
            }
            styleName = styleNameNode->getValue();
            styleNameNode = styleNameNode->getNext();
            if (!isNodeNull(styleNameNode)) {
                styleValue = convertStyleNodeToStyleValue(styleNameNode);
                if (styleValue != nullptr) {
                    (*appliedStyleMap)[styleName] = StyleRule{styleValue, true, 0, fileNumber, *ruleNumber};
                    (*ruleNumber)++;
                }
            }
        }
        else if (token == Token::StyleBlock) {
            convertStyleBlock(fileNumber, ruleNumber);
        }
        // if other token type, just don't use the definition
        definition = definition->getNext();
    }

    return appliedStyleMap;
}

std::list<StyleBlock *> *NodeToStyleComponentList::createStyleComponents(std::list<std::list<StyleComponentDataList *> *>::const_iterator componentsListIt, StyleComponentDataList *components, StyleValuesMap *appliedStyleMap) {
    std::list<StyleBlock *> *styleComponentList = new std::list<StyleBlock *>();
    std::list<StyleBlock *> *tmpStyleComponentList;
    StyleComponentDataList::const_iterator componentsIt;
    if (components == nullptr) return nullptr;
    bool islistHead = std::next(componentsListIt) == requiredStyleComponentsLists.cend();
    if (islistHead) { // if at end of the declaration list
        for (StyleComponentDataList *components : **componentsListIt) {
            styleComponentList->push_back(new StyleBlock(components, appliedStyleMap));
        }
    }
    else {
        for (StyleComponentDataList *componentsList : **componentsListIt) {
            componentsIt = std::prev(components->end());
            components->splice(components->end(), *componentsList);

            tmpStyleComponentList = createStyleComponents(std::next(componentsListIt), components, appliedStyleMap);
            if (tmpStyleComponentList != nullptr) {
                styleComponentList->splice(styleComponentList->end(), *tmpStyleComponentList);
                delete tmpStyleComponentList;
            }
            components->erase(componentsIt);
        }
    }
    return styleComponentList;
}

void NodeToStyleComponentList::convertStyleBlock(int fileNumber, int *ruleNumber) {
    std::list<StyleComponentDataList *> *styleComponentsLists;
    if (tree == nullptr || tree->getTokenType() != Token::StyleBlock) return;
    tree = tree->getChild();
    styleComponentsLists = convertStyleComponents();
    if (styleComponentsLists == nullptr || styleComponentsLists->empty()) {
        delete styleComponentsLists;
        return;
    }
    tree = tree->getNext();

    StyleValuesMap *appliedStyleMap = convertAppliedStyle(fileNumber, ruleNumber);
    tree = tree->getParent();
    if (appliedStyleMap == nullptr || appliedStyleMap->empty()) {
        delete styleComponentsLists;
        delete appliedStyleMap;
        return;
    }
    requiredStyleComponentsLists.push_back(styleComponentsLists);
    StyleComponentDataList components = StyleComponentDataList();
    std::list<StyleBlock *> *finalStyleComponents = createStyleComponents(requiredStyleComponentsLists.cbegin(), &components, appliedStyleMap);
    delete appliedStyleMap;
    for (StyleComponentDataList *componentDataList : *(requiredStyleComponentsLists.back())) {
        delete componentDataList;
    }
    delete requiredStyleComponentsLists.back();
    requiredStyleComponentsLists.pop_back();
    if (finalStyleComponents != nullptr) {
        styleDefinitions->splice(styleDefinitions->end(), *finalStyleComponents);
        delete finalStyleComponents;
    }
}

std::list<StyleBlock *> *NodeToStyleComponentList::convert(Node *styleTree, int fileNumber, int *ruleNumber) {
    *ruleNumber = 0;
    if (styleTree->getTokenType() != Token::NullRoot) return nullptr;
    styleDefinitions = new std::list<StyleBlock *>();
    requiredStyleComponentsLists = std::list<std::list<StyleComponentDataList *> *>();

    tree = styleTree->getChild();
    while (tree != nullptr && tree->getTokenType() == Token::StyleBlock) { // FIXME: also verified in the convertStyleBlock method. Don't need to check twice
        convertStyleBlock(fileNumber, ruleNumber);
        tree = tree->getNext();
    }
    return styleDefinitions;
}