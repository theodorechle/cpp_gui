#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "abstract_manager.hpp"
#include "../elements/ui_element.hpp"

#include <list>
#include <algorithm>

class UIManager: AbstractManager {
    std::list<UIElement*> elementsList;
public:
    UIManager();
    void addElement(UIElement* element);
    void removeElement(UIElement* element, bool delete_if_found=true);
    void removeAllElements(bool delete_if_found=true) override;
    void render() override;
};

#endif // UIMANAGER_HPP
