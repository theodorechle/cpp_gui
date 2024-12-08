#ifndef U_I_ELEMENT_HPP
#define U_I_ELEMENT_HPP

#include "abstract_element.hpp"
#include <iostream>

class UIElement: public AbstractElement {
    void computeLayout() override final;
public:
    UIElement(): AbstractElement{} {}
};

#endif // U_I_ELEMENT_HPP
