#ifndef NODE_TO_STYLE_COMPONENT_LIST_HPP
#define NODE_TO_STYLE_COMPONENT_LIST_HPP

#include "node.hpp"
#include "style_component.hpp"

#include <iterator>
#include <list>
#include <tuple>

class NodeToStyleComponentList {
    Node *tree;
    // for each inner style block, multiple components list definitions (separated by commas)
    std::list<std::list<StyleComponentDataList *> *> requiredStyleComponentsLists;
    std::list<StyleComponent *> *styleDefinitions;

    std::list<StyleComponentDataList *> *convertStyleComponents();

    /**
     * Be aware that the method does not check if the given npde is instanciated for performance reasons
     */
    StyleValue * convertStyleNodesToAppliedStyle(Node *node);

    StyleValuesMap *convertAppliedStyle(int fileNumber, int *ruleNumber);
    /**
     * Does not accept an null pointer as components
     */
    std::list<StyleComponent *> *createStyleComponents(std::list<std::list<StyleComponentDataList *> *>::const_iterator componentsListIt, StyleComponentDataList *components, StyleValuesMap *appliedStyle);
    void convertStyleBlock(int fileNumber, int *ruleNumber);

public:
    std::list<StyleComponent *> *convert(Node *tree, int fileNumber, int *ruleNumber);
};

#endif // NODE_TO_STYLE_COMPONENT_LIST_HPP
