#include "abstract_manager.hpp"

namespace gui {
    namespace element {
        namespace manager {

            AbstractManager::~AbstractManager() { deleteElementsTree(); }

            void AbstractManager::setElementsTree(gui::element::AbstractElement *element) { elementsTree = element; }

            void AbstractManager::deleteElementsTree() { delete elementsTree; }

            void AbstractManager::render() {
                computeLayout();
                if (!needRendering()) return;
                needRendering(false);
                renderElements();
            }

            void AbstractManager::computeLayout() {
                if (!needRecompute()) return;
                needRecompute(false);
                computeElementsLayout();
                needRendering(true); // TODO: ask rendering onyl if layout changed
            }

        } // namespace manager
    } // namespace element
} // namespace gui
