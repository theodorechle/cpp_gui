#ifndef ABSTRACT_MANAGER_HPP
#define ABSTRACT_MANAGER_HPP


#include <list>
#include <algorithm>

class AbstractElement;

class AbstractManager {
protected:
    std::list<AbstractElement*> elementsList = std::list<AbstractElement*>();
public:
    void addElement(AbstractElement* element);
    void removeElement(AbstractElement* element, bool delete_if_found=true);
    void removeAllElements(bool delete_if_found=true);
    virtual void render() = 0;
    virtual ~AbstractManager() = default;
};

#include "../elements/abstract_element.hpp"

#endif // ABSTRACT_MANAGER_HPP
