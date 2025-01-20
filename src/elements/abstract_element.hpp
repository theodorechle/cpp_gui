#ifndef ABSTRACT_ELEMENT_HPP
#define ABSTRACT_ELEMENT_HPP

#include "../elements_style/elements_style_manager.hpp"
#include <string>
#include <vector>

class AbstractElement {
    std::string elementName;
    AbstractElement *parent = nullptr;
    AbstractElement *child = nullptr;
    AbstractElement *next = nullptr;
    ElementsStyleManager *elementsStyleManager;
    virtual void computeLayout() = 0;

protected:
    ElementStyle *elementStyle;
    void setParent(AbstractElement *parent);
    void updateStyle();

    /**
     * Override this function to draw self element.
     * Only draw element content (no border / background)
     */
    virtual void renderSelfBeforeChilds() const {};

    /**
     * Override this function to draw self element.
     * Only draw element content (no border / background)
     */
    virtual void renderSelfAfterChilds() const {};

    /**
     * Should call each child with a portion of the element surface
     */
    virtual void renderChilds() {};

public:
    /**
     * If no style manager is given, the element can't have style
     */
    AbstractElement(std::string elementName, ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                    const std::string &identifier = "");
    const std::string &getName() const { return elementName; }
    AbstractElement *getParent() { return parent; }
    void addChild(AbstractElement *child);
    AbstractElement *getChild() { return child; }
    void setNext(AbstractElement *next) { this->next = next; }
    AbstractElement *getNext() { return next; }
    virtual ~AbstractElement();

    virtual void render() = 0;
};

#endif // ABSTRACT_ELEMENT_HPP