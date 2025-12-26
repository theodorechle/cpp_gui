#include "abstract_manager.hpp"

namespace gui::element::manager {
    void AbstractManager::addChildToRootElement(AbstractElement *childElement) { elementsTree->addChild(childElement); }

    void AbstractManager::needUpdate(AbstractElement *element) {
        if (elementsToUpdate.find(element) != elementsToUpdate.cend()) return;
        if (!element) {
            elementsToUpdate.clear();
            return;
        }
        elementsToUpdate.insert(element);
    }

    AbstractManager::~AbstractManager() { deleteElementsTree(); }

    void AbstractManager::setSubRootElement(AbstractElement *element) {
        if (elementsTree == nullptr) {
            createRootElement();
            needUpdate(elementsTree);
        }
        else {
            needUpdate(element);
        }
        addChildToRootElement(element);
    }

    void AbstractManager::elementEvent(event::ElementEvent event, AbstractElement *caller) {
        switch (event) {
        case event::ElementEvent::REMOVE_CHILDS:
        case event::ElementEvent::ADD_CHILD:
        case event::ElementEvent::CONTENT_CHANGED:
            needUpdate(caller);
            break;
        default:
            break;
        }
    }

    void AbstractManager::deleteElementsTree() { delete elementsTree; }

    void AbstractManager::render(bool clear) {
        if (_currentStatus == Status::NOT_STARTED) {
            if (_styleManager && elementsTree) {
                _styleManager->rootElement(elementsTree);
                // TODO: style should be applied to elementsTree from here, they don't need to know the styleManager
                elementsTree->updateStyle();
            }
            _currentStatus = Status::RUNNING;
        }
        if (needUpdate()) {
            update();
            renderElements(clear);
            needUpdate(nullptr);
        };
    }

    // FIXME: not sure this function should be keeped as one function
    // also, sendEvent function is defined in ui_manager but could be here
    void AbstractManager::setElementsModifierState(const std::string &modifier, AbstractElement *leafElement, bool enabled, const event::Event *event) {
        AbstractElement *element = leafElement;
        while (element != nullptr) {
            element->setModifierState(modifier, enabled);
            element->updateStyle();
            element->catchEvent(event);
            element = element->parent();
        }
        needUpdate(leafElement);
    }

} // namespace gui::element::manager
