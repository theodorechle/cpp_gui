#include "abstract_element.hpp"

namespace gui::element {
    void AbstractElement::setParent(AbstractElement *parent) {
        commons::Node<AbstractElement>::parent(parent);
        updateStyle();
    }

    void AbstractElement::updateStyle() {
        if (elementsStyleManager != nullptr) {
            elementsStyleManager->applyStyleToElement(this);
        }
        if (child() != nullptr) child()->updateStyle();
        if (next() != nullptr) next()->updateStyle();
    }

    void AbstractElement::sendEventToManager(ElementEvent event) {
        if (_manager == nullptr) return;
        _manager->elementEvent(event, this);
    }

    AbstractElement::AbstractElement(std::string elementName, elementStyle::manager::StyleManager *elementsStyleManager,
                                     std::vector<std::string> *classes, const std::string &identifier)
        : elementName{elementName}, elementsStyleManager{elementsStyleManager} {

        _style = elementStyle::ElementStyle();
        _style.addSelector(elementName, style::StyleComponentType::ElementName);
        // set selectors
        if (classes != nullptr) {
            for (std::string c : *classes) {
                _style.addSelector(c, style::StyleComponentType::Class);
            }
        }
        if (!identifier.empty()) _style.addSelector(identifier, style::StyleComponentType::Identifier);
        if (elementsStyleManager == nullptr) return;
    }

    void AbstractElement::addChild(AbstractElement *newChild) {
        if (newChild == nullptr) return;
        commons::Node<AbstractElement>::addChild(newChild);
        newChild->manager(_manager);
        sendEventToManager(ElementEvent::ADD_CHILD);
    }

    void AbstractElement::removeChilds() {
        commons::Node<AbstractElement>::removeChilds();
        sendEventToManager(ElementEvent::REMOVE_CHILDS);
    }

    void AbstractElement::manager(manager::AbstractManager *manager) {
        _manager = manager;
        if (child()) child()->manager(manager);
        if (next()) next()->manager(manager);
    }

    void AbstractElement::displayStyle() {
        std::cerr << "########################################################\n";
        std::cerr << "path in tree:\n";
        const AbstractElement *node = this;
        std::list<std::list<std::string>> path = {};
        while (node->parent() != nullptr) {
            path.push_front(_style.debugToString());
            node = node->parent();
        }

        std::cerr << "selectors:\n";
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

        std::cerr << "applied rules:\n";
        for (std::pair<std::string, style::StyleRule> rule : _style.rules()) {
            std::cerr << rule.first << " -> ";
            style::StyleValue *value = rule.second.value;
            std::cerr << value->value();
            value = value->child();
            while (value != nullptr) {
                std::cerr << value->value();
                value = value->next();
            }
            std::cerr << "\n";
        }
        std::cerr << "########################################################\n";
    }

    void AbstractElement::setModifierState(std::string modifier, bool enabled) {
        if (enabled) _style.addSelector(modifier, style::StyleComponentType::Modifier);
        else _style.removeSelector(modifier, style::StyleComponentType::Modifier);
    }

    std::string AbstractElement::debugValue() const {
        std::stringstream stringStream;
        stringStream << "(" << this << ") " << elementName;
        return stringStream.str();
    }

} // namespace gui::element