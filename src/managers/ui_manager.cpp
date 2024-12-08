#include "ui_manager.hpp"

UIManager::UIManager() {
    
}

void UIManager::render() {
    for (AbstractElement* element : elementsList) {
        element->render();
    }
}