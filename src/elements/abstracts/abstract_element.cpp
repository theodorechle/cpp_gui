#include "abstract_element.hpp"

namespace gui::element {
    void AbstractElement::parent(AbstractElement *parent) {
        this->_parent = parent;
        updateStyle();
    }

    void AbstractElement::updateStyle() {
        if (elementsStyleManager != nullptr) {
            elementsStyleManager->applyStyleToElement(style);
        }
    }

    void AbstractElement::sendEventToManager(ElementEvent event) {
        if (elementManager == nullptr) return;
        elementManager->elementEvent(event, this);
    }

    AbstractElement::AbstractElement(std::string elementName, elementStyle::manager::StyleNodesManager *elementsStyleManager,
                                     std::vector<std::string> *classes, const std::string &identifier)
        : elementName{elementName}, elementsStyleManager{elementsStyleManager} {

        style = new gui::elementStyle::StyleNode();
        if (elementsStyleManager != nullptr) {
            style->setFontsPath(elementsStyleManager->getFontsPath());
        }

        style->addSelector(elementName, style::StyleComponentType::ElementName);
        // set selectors
        if (classes != nullptr) {
            for (std::string c : *classes) {
                style->addSelector(c, style::StyleComponentType::Class);
            }
        }
        if (!identifier.empty()) style->addSelector(identifier, style::StyleComponentType::Identifier);
        if (elementsStyleManager == nullptr) return;
        elementsStyleManager->addElementStyle(style);
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
        style->addChild(newChild->style);
        newChild->parent(this);
        _nbChilds++;
    }

    void AbstractElement::removeChilds() {
        _child = nullptr;
        style->removeChilds();
        sendEventToManager(ElementEvent::REMOVE_CHILDS);
    }

    void AbstractElement::manager(manager::AbstractManager *manager) { elementManager = manager; }

    AbstractElement::~AbstractElement() {
        delete style;
        delete _child;
        delete _next;
    }

    void AbstractElement::setModifierState(std::string modifierName, bool enabled) { style->setModifierState(modifierName, enabled); }

    void AbstractElement::displayStyle() {
        std::cerr << "########################################################\n";
        std::cerr << "path in tree:\n";
        const AbstractElement *node = this;
        std::list<std::list<std::string>> path = {};
        while (node->_parent != nullptr) {
            std::list<std::string> pathFragment = {};
            for (style::StyleComponentData selector : *(node->style->getSelectors())) {
                char firstChar[2] = "";
                switch (selector.second) {
                case style::StyleComponentType::Identifier:
                    firstChar[0] = '#';
                    break;
                case style::StyleComponentType::Class:
                    firstChar[0] = '.';
                    break;
                case style::StyleComponentType::Modifier:
                    firstChar[0] = ':';
                    break;
                default:
                    break;
                }
                pathFragment.push_back(firstChar + selector.first);
            }
            path.push_front(pathFragment);
            node = node->_parent;
        }
        for (const std::list<std::string> &path_fragment : path) {
            std::cerr << "/";
            std::size_t index = 0;
            for (const std::string &frag : path_fragment) {
                std::cerr << frag;
                if (index < path_fragment.size() - 1) std::cerr << "|";
                index++;
            }
        }

        std::cerr << "\n";

        std::cerr << "selectors:\n";
        for (style::StyleComponentData selector : *(style->getSelectors())) {
            std::cerr << "(" << style::styleComponentTypeToString(selector.second) << ") " << selector.first << "\n";
        }
        std::cerr << "applied rules:\n";
        for (std::pair<std::string, elementStyle::StyleRules> rule : style->getStyle()) {
            std::cerr << rule.first << " -> ";
            style::StyleValue *value = rule.second.front().value;
            std::cerr << value->getValue();
            value = value->getChild();
            while (value != nullptr) {
                std::cerr << value->getValue();
                value = value->getNext();
            }
            std::cerr << "\n";
        }
        std::cerr << "########################################################\n";
    }
}