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

    std::list<StyleComponentDataList *> *convertStyleComponents();
    AppliedStyleMap *convertAppliedStyle();
    /**
     * Does not accept an null pointer as components
     */
    std::list<StyleComponent *> *createStyleComponents(std::list<std::list<StyleComponentDataList *> *>::const_iterator componentsListIt, StyleComponentDataList *components, AppliedStyleMap *appliedStyle);
    std::list<StyleComponent *> *convertStyleBlock();

public:
    std::list<StyleComponent *> *convert(Node *tree);
};

#endif // NODE_TO_STYLE_COMPONENT_LIST_HPP
