#ifndef ABSTRACT_ELEMENT_HPP
#define ABSTRACT_ELEMENT_HPP

#include <SDL3/SDL.h>

class AbstractElement {
private:
    virtual void computeLayout() = 0;
public:
    virtual ~AbstractElement() = default;
    virtual void render() const = 0;

};

#endif // ABSTRACT_ELEMENT_HPP