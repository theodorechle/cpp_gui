#ifndef ABSTRACT_MANAGER_HPP
#define ABSTRACT_MANAGER_HPP


#include <list>
#include <algorithm>

class AbstractElement;

class AbstractManager {
protected:
    std::list<AbstractElement*> elementsList;
private:
    virtual void addElement(AbstractElement* element) = 0;
    virtual void removeElement(AbstractElement* element, bool delete_if_found=true) = 0;
    virtual void removeAllElements(bool delete_if_found=true) = 0;
    virtual void render() = 0;
};

#include "../elements/abstract_element.hpp"

#endif // ABSTRACT_MANAGER_HPP
