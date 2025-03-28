#include "abstract_element.hpp"

namespace gui {
    namespace element {

        void AbstractElement::parent(AbstractElement *parent) {
            this->_parent = parent;
            updateStyle();
        }

        void AbstractElement::updateStyle() {
            if (elementsStyleManager != nullptr) {
                elementsStyleManager->applyStyleToElement(elementStyle);
            }
        }

        AbstractElement::AbstractElement(std::string elementName, elementStyle::manager::StyleNodesManager *elementsStyleManager,
                                         std::vector<std::string> *classes, const std::string &identifier)
            : elementName{elementName}, elementsStyleManager{elementsStyleManager} {

            elementStyle = new gui::elementStyle::StyleNode();
            if (elementsStyleManager != nullptr) {
                elementStyle->setFontsPath(elementsStyleManager->getFontsPath());
            }

            elementStyle->addSelector(elementName, style::StyleComponentType::ElementName);
            // set selectors
            if (classes != nullptr) {
                for (std::string c : *classes) {
                    elementStyle->addSelector(c, style::StyleComponentType::Class);
                }
            }
            if (!identifier.empty()) elementStyle->addSelector(identifier, style::StyleComponentType::Identifier);
            if (elementsStyleManager == nullptr) return;
            elementsStyleManager->addElementStyle(elementStyle);
        }

        void AbstractElement::addChild(AbstractElement *newChild) {
            if (newChild == nullptr) return;
            AbstractElement *nextChild = child();
            AbstractElement *selfChild;
            if (nextChild == nullptr) {
                this->_child = newChild;
            }
            else {
                do {
                    selfChild = nextChild;
                    nextChild = selfChild->next();
                } while (nextChild != nullptr);
                selfChild->next(newChild);
            }
            if (newChild != nullptr) {
                elementStyle->addChild(newChild->elementStyle);
                newChild->parent(this);
                _nbChilds++;
            }
        }

        void AbstractElement::removeChilds() {
            _child = nullptr;
            elementStyle->removeChilds();
        }

        AbstractElement::~AbstractElement() {
            delete elementStyle;
            delete _child;
            delete _next;
        }

        void AbstractElement::setModifierState(std::string modifierName, bool enabled) { elementStyle->setModifierState(modifierName, enabled); }

    } // namespace element
} // namespace gui
