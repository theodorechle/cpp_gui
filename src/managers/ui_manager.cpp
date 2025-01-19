#include "ui_manager.hpp"

void UIManager::setElementsTree(UIElement* element) {
    return AbstractManager::setElementsTree(element);
}

UIElement *UIManager::removeElementsTree() {
    return static_cast<UIElement *>(AbstractManager::removeElementsTree());
}

void UIManager::render() const {
    static_cast<UIElement *>(elementsTree)->setRenderer(renderer);
    elementsTree->render();
}