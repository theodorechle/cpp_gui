#include "abstract_manager.hpp"

namespace gui::element::manager {
    void AbstractManager::addChildToRootElement(AbstractElement *childElement) { elementsTree->addChild(childElement); }

    void AbstractManager::needUpdate(AbstractElement *element) {
        if (elementsToUpdate.find(element) != elementsToUpdate.cend()) return;
        if (!element) {
            elementsToUpdate.clear();
            return;
        }
        // if (!elementsToUpdate.size() || !element->constParent()) {
        //     elementsToUpdate.insert(element);
        //     return;
        // }

        // std::list<const AbstractElement *> pathNewElement;
        // const AbstractElement *parent = element;
        // while (parent) {
        //     pathNewElement.push_front(parent);
        //     parent = parent->constParent();
        // }

        // for (const AbstractElement *elementToUpdate : elementsToUpdate) {
        // std::list<const AbstractElement *> pathCurrentElement;

        // while (elementToUpdate) {
        //     pathCurrentElement.push_front(elementToUpdate);
        //     elementToUpdate = elementToUpdate->constParent();
        // }

        // // check if one element contains the other (in which case it don't need to be added to the elements who need to be updated since it will
        // automatically be) std::list<const AbstractElement *>::const_iterator newElementIt = pathNewElement.cbegin(); while ((*newElementIt) !=
        // elementToUpdate) {
        //     newElementIt++;
        //     if (newElementIt != pathNewElement.cend()) return;
        // }

        // std::list<const AbstractElement *>::const_iterator currentElementIt = pathCurrentElement.cbegin();
        // while ((*currentElementIt) != element) {
        //     currentElementIt++;
        //     if (currentElementIt != pathCurrentElement.cend()) return;
        // }

        // // std::list<const AbstractElement *>::const_iterator newElementIt = pathNewElement.cbegin();
        // // while ((*newElementIt) != pathCurrentElement.front()) newElementIt++;
        // // if ((*newElementIt) == pathCurrentElement.front()) return;

        // // std::list<const AbstractElement *>::const_iterator currentElementIt = pathCurrentElement.cbegin();
        // // while ((*currentElementIt) != pathNewElement.front()) currentElementIt++;
        // // if ((*currentElementIt) == pathNewElement.front()) return;
        //     elementsToUpdate.insert(element);
        // }
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
