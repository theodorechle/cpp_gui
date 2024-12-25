#include "abstract_element.hpp"

AbstractElement::AbstractElement(std::string elementName, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes, const std::string &identifier, AbstractElement *parent, AbstractElement *child, AbstractElement *next)
    : elementName{elementName}, parent{parent}, child{child}, next{next} {

    style = new ElementStyle();
    if (parent != nullptr) style->setParent(parent->style);
    if (child != nullptr) style->setChild(child->style);
    if (next != nullptr) style->setNext(next->style);

    style->addSelector(elementName, StyleComponentType::ElementName);
    // set selectors
    if (classes != nullptr) {
        for (std::string c : *classes) {
            style->addSelector(c, StyleComponentType::Class);
        }
    }
    if (!identifier.empty()) style->addSelector(identifier, StyleComponentType::Identifier);

    if (elementsStyleManager != nullptr) {
        elementsStyleManager->addElementStyle(style);
    }
}
