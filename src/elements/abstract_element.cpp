#include "abstract_element.hpp"

AbstractElement::AbstractElement(std::string elementName, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                                 const std::string &identifier, AbstractElement *parent, AbstractElement *child, AbstractElement *next)
    : elementName{elementName}, parent{parent}, child{child}, next{next} {

    elementStyle = new ElementStyle();
    if (parent != nullptr) elementStyle->setParent(parent->elementStyle);
    if (child != nullptr) elementStyle->setChild(child->elementStyle);
    if (next != nullptr) elementStyle->setNext(next->elementStyle);

    elementStyle->addSelector(elementName, StyleComponentType::ElementName);
    // set selectors
    if (classes != nullptr) {
        for (std::string c : *classes) {
            elementStyle->addSelector(c, StyleComponentType::Class);
        }
    }
    if (!identifier.empty()) elementStyle->addSelector(identifier, StyleComponentType::Identifier);

    if (elementsStyleManager != nullptr) {
        elementsStyleManager->addElementStyle(elementStyle);
    }
}
