#ifndef ABSTRACT_MANAGER_HPP
#define ABSTRACT_MANAGER_HPP

#include <list>
#include <algorithm>
#include <SDL3/SDL.h>

class AbstractElement;

class AbstractManager {
protected:
    std::list<AbstractElement*> elementsList = std::list<AbstractElement*>();
public:
    virtual ~AbstractManager() = default;
    void addElement(AbstractElement* element);
    void removeElement(AbstractElement* element, bool delete_if_found=true);
    void removeAllElements(bool delete_if_found=true);
    virtual void render() const = 0;
};

#include "../elements/abstract_element.hpp"

#endif // ABSTRACT_MANAGER_HPP
