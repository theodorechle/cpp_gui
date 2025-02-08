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
                if (!needRecomputeLayout()) return;
                needRecomputeLayout(false);
                computeElementsLayout();
                needRendering(true); // TODO: ask rendering only if layout changed
            }

        } // namespace manager
    } // namespace element
} // namespace gui
