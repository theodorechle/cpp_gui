#ifndef ABSTRACT_ELEMENT_HPP
#define ABSTRACT_ELEMENT_HPP

#include "../managers/abstract_manager.hpp"

class AbstractElement {
protected:
    AbstractManager *uiManager;
private:
    virtual void computeLayout() = 0;
    virtual void render() const = 0;
public:
    virtual ~AbstractElement() = 0;
};

#endif // ABSTRACT_ELEMENT_HPP
