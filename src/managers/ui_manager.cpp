#include "ui_manager.hpp"

void UIManager::render() const {
    for (AbstractElement* element : elementsList) {
        element->render();
    }
}