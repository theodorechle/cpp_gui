#include "ui_manager.hpp"

UIManager::UIManager() {
    
}

void UIManager::addElement(UIElement* element) {
    elementsList.push_back(element);
}

void UIManager::removeElement(UIElement* element, bool delete_if_found) {
    if (delete_if_found && std::find(elementsList.cbegin(), elementsList.cend(), element) != elementsList.cend()) {
        delete element;
    }
    elementsList.remove(element);
}

void UIManager::removeAllElements(bool delete_if_found) {
    if (delete_if_found) {
        for (UIElement* element : elementsList) {
            delete element;
        }
    }
    elementsList.clear();
}