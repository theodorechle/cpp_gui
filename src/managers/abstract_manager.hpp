#ifndef ABSTRACT_MANAGER_HPP
#define ABSTRACT_MANAGER_HPP

#include <list>
#include <algorithm>

class AbstractElement;

class AbstractManager {
protected:
    AbstractElement* elementsTree = nullptr;
public:
    virtual ~AbstractManager() = default;
    void setElementsTree(AbstractElement* element);
    AbstractElement *removeElementsTree();
    virtual void render() const = 0;
};

#include "../elements/abstract_element.hpp"

#endif // ABSTRACT_MANAGER_HPP
