#include "abstract_manager.hpp"

namespace gui {
    namespace element {
        namespace manager {

            AbstractManager::~AbstractManager() { deleteElementsTree(); }

            void AbstractManager::setElementsTree(gui::element::AbstractElement *element) { elementsTree = element; }

            void AbstractManager::deleteElementsTree() { delete elementsTree; }

            void AbstractManager::render() {
                if (!needRendering()) return;
                needRendering(false);
                renderElements();
            }

        } // namespace manager
    } // namespace element
} // namespace gui
