#include "abstract_manager.hpp"

namespace gui {
    namespace element {
        namespace manager {

            AbstractManager::~AbstractManager() { deleteElementsTree(); }

            void AbstractManager::setElementsTree(gui::element::AbstractElement *element) { elementsTree = element; }

            void AbstractManager::deleteElementsTree() { delete elementsTree; }

            void AbstractManager::render(bool clear) {
                computeLayout();
                if (!needRendering()) return;
                needRendering(false);
                renderElements(clear);
            }

            void AbstractManager::computeLayout() {
                if (!needRecomputeLayout()) return;
                needRecomputeLayout(false);
                computeElementsLayout();
                needRendering(true); // TODO: remove, should be done by elements using manager_actions_service
            }

        } // namespace manager
    } // namespace element
} // namespace gui
