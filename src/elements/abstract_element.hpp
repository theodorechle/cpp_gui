#ifndef ABSTRACT_ELEMENT_HPP
#define ABSTRACT_ELEMENT_HPP

class AbstractElement {
    virtual void computeLayout() = 0;
    virtual void render() const = 0;
};

#endif // ABSTRACT_ELEMENT_HPP
