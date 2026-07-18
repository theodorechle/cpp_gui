#include "abstract_manager.hpp"
#include "abstract_element.hpp"

namespace gui::element::manager {
    void AbstractManager::addChildToRootElement(AbstractElement *childElement) { elementsTree->addChild(childElement); }

    void AbstractManager::needUpdate(AbstractElement *element) { elementsToUpdate.insert(element); }

    void AbstractManager::update() { updateModifiedElements(); }

    void AbstractManager::setElementsModifierState(const std::string &modifier, AbstractElement *leafElement, bool enabled,
                                                   const event::Event *event) {
        AbstractElement *element = leafElement;
        while (element != nullptr) {
            element->setModifierState(modifier, enabled);
            element->updateStyle();
            sendEventToElement(event, element);
            element = element->parent();
        }
    }

    void AbstractManager::sendEventToElementAndAncestors(const event::Event *event, AbstractElement *leafElement) {
        AbstractElement *element = leafElement;
        while (element != nullptr) {
            sendEventToElement(event, element);
            element = element->parent();
        }
    }

    void AbstractManager::sendEventToElement(const event::Event *event, AbstractElement *element) { element->catchEvent(event->copy()); }

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

    void AbstractManager::letElementsHandleEvents(AbstractElement *element) {
        // TODO: update when threads will be implemented
        AbstractElement *currentElement = element;
        while (currentElement != nullptr) {
            while (currentElement->eventInQueue()) {
                currentElement->handleNextEvent();
            }
            letElementsHandleEvents(currentElement->child());
            currentElement = currentElement->next();
        }
    }

    void AbstractManager::render(bool clear) {
        if (_currentStatus == Status::NOT_STARTED) { // move elsewhere to not run the condition on every call
            if (_styleManager && elementsTree) {
                _styleManager->rootElement(elementsTree);
                // TODO: style should be applied to elementsTree from here, they don't need to know the styleManager
                elementsTree->updateStyle();
            }
            _currentStatus = Status::RUNNING;
        }

        // TODO: update when threads will be implemented
        letElementsHandleEvents(elementsTree);

        if (needUpdate()) {
            update();
            renderElements(clear);
        };
    }

} // namespace gui::element::manager
