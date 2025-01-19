#include "abstract_manager.hpp"

void AbstractManager::setElementsTree(AbstractElement* element) {
    elementsTree = element;
}

AbstractElement *AbstractManager::removeElementsTree() {
    return elementsTree;
}
