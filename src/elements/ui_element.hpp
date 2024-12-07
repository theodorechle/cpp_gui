#ifndef U_I_ELEMENT_HPP
#define U_I_ELEMENT_HPP

#include "abstract_element.hpp"

class UIElement: public AbstractElement {
    void computeLayout() override final;
public:
    virtual ~UIElement();
    void render() const override;
};

#endif // U_I_ELEMENT_HPP
