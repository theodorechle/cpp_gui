#include "abstract_manager.hpp"

namespace gui {
    namespace element {
        namespace manager {
            AbstractManager::~AbstractManager() { deleteElementsTree(); }

            void AbstractManager::setSubRootElement(gui::element::AbstractElement *element) {
                elementsTree = createRootElement();
                elementsTree->addChild(element);
            }

            void AbstractManager::deleteElementsTree() { delete elementsTree; }

            void AbstractManager::render(bool clear) {
                if (!needUpdate()) return;
                needUpdate(false);
                renderElements(clear);
            }
        } // namespace manager
    } // namespace element
} // namespace gui
