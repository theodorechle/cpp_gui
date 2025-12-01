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

    void AbstractManager::elementEvent(ElementEvent event, AbstractElement *caller) {
        switch (event) {
        case ElementEvent::REMOVE_CHILDS:
        case ElementEvent::ADD_CHILD:
        case ElementEvent::CONTENT_CHANGED:
            needUpdate(caller);
            break;
        default:
            break;
        }
    }

    void AbstractManager::deleteElementsTree() { delete elementsTree; }

    void AbstractManager::render(bool clear) {
        if (needUpdate()) {
            update();
            needUpdate(nullptr);
        };
        renderElements(clear);
    }
} // namespace gui::element::manager
