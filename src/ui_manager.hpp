#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "ui_element.hpp"
#include <list>
#include <algorithm>

class UIManager {
    std::list<UIElement*> elementsList;
public:
    UIManager();
    void addElement(UIElement* element);
    void removeElement(UIElement* element, bool delete_if_found=true);
    void removeAllElements(bool delete_if_found=true);
    void refresh();
};

#endif // UIMANAGER_HPP
