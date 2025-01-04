#ifndef ABSTRACT_ELEMENT_HPP
#define ABSTRACT_ELEMENT_HPP

#include "../elements_style/elements_style_manager.hpp"
#include <string>
#include <vector>

class AbstractElement {
    std::string elementName;
    AbstractElement *parent;
    AbstractElement *child;
    AbstractElement *next;
    virtual void computeLayout() = 0;

protected:
    ElementStyle *elementStyle;

public:
    /**
     * If no style manager is given, the element can't have style
     */
    AbstractElement(std::string elementName, ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                    const std::string &identifier = "", AbstractElement *parent = nullptr, AbstractElement *child = nullptr,
                    AbstractElement *next = nullptr);
    void setParent(AbstractElement *parent) { this->parent = parent; }
    AbstractElement *getParent() { return this->parent; }
    void setChild(AbstractElement *child) { this->child = child; }
    AbstractElement *getChild() { return this->child; }
    void setNext(AbstractElement *next) { this->next = next; }
    AbstractElement *getNext() { return this->next; }
    virtual ~AbstractElement() = default;
    virtual void render() const = 0;
};

#endif // ABSTRACT_ELEMENT_HPP