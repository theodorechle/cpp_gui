#ifndef U_I_ELEMENT_HPP
#define U_I_ELEMENT_HPP

#include "abstract_element.hpp"

class UIElement: AbstractElement {
    void computeLayout() override final;
public:
    virtual ~UIElement();
};

#endif // U_I_ELEMENT_HPP
