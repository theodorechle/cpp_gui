#ifndef ABSTRACT_ELEMENT_HPP
#define ABSTRACT_ELEMENT_HPP

#include "../managers/abstract_manager.hpp"

class AbstractElement {
protected:
    AbstractManager *manager;
private:
    virtual void computeLayout() = 0;
public:
    AbstractElement(AbstractManager *manager);
    virtual void render() const = 0;
    virtual ~AbstractElement() = default;
};

#endif // ABSTRACT_ELEMENT_HPP
