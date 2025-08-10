#include "abstract_manager.hpp"

namespace gui::element::manager {
    void AbstractManager::addChildToRootElement(gui::element::AbstractElement *childElement) { elementsTree->addChild(childElement); }

    AbstractManager::~AbstractManager() { deleteElementsTree(); }

    void AbstractManager::setSubRootElement(gui::element::AbstractElement *element) {
        createRootElement();
        addChildToRootElement(element);
    }

    void AbstractManager::elementEvent(ElementEvent event, AbstractElement *caller) {
        switch (event)
        {
        case ElementEvent::REMOVE_CHILDS:
            needUpdate(true);
            break;
        default:
            break;
        }
    }

    void AbstractManager::deleteElementsTree() { delete elementsTree; }

    void AbstractManager::render(bool clear) {
        if (needUpdate()) {
            update();
            needUpdate(false);
        };
        renderElements(clear);
    }
}
