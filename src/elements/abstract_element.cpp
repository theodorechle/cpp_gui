#include "abstract_element.hpp"

void AbstractElement::setParent(AbstractElement *parent) {
     this->parent = parent;
     updateStyle();
}

void AbstractElement::updateStyle() {
    if (elementsStyleManager != nullptr) {
        elementsStyleManager->applyStyleToElement(elementStyle);
    }
}

AbstractElement::AbstractElement(std::string elementName, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                                 const std::string &identifier)
    : elementName{elementName}, elementsStyleManager{elementsStyleManager} {

    elementStyle = new ElementStyle();

    elementStyle->addSelector(elementName, StyleComponentType::ElementName);
    // set selectors
    if (classes != nullptr) {
        for (std::string c : *classes) {
            elementStyle->addSelector(c, StyleComponentType::Class);
        }
    }
    if (!identifier.empty()) elementStyle->addSelector(identifier, StyleComponentType::Identifier);
    elementsStyleManager->addElementStyle(elementStyle);
}

void AbstractElement::addChild(AbstractElement *child) {
    AbstractElement *nextChild = getChild();
    AbstractElement *selfChild;
    if (nextChild == nullptr) {
        this->child = child;
    }
    else {
        do {
            selfChild = nextChild;
            nextChild = selfChild->getNext();
        } while (nextChild != nullptr);
        selfChild->setNext(child);
    }
    elementStyle->addChild(child->elementStyle);
    if (child == nullptr) return;
    child->setParent(this);
}

AbstractElement::~AbstractElement() {
    delete elementStyle;
    delete child;
    delete next;
}
