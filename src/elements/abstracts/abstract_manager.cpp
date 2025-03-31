#include "abstract_manager.hpp"

namespace gui {
    namespace element {
        namespace manager {
            void AbstractManager::addChildToRootElement(gui::element::AbstractElement *childElement) {
                elementsTree->addChild(childElement);

            }

            AbstractManager::~AbstractManager() { deleteElementsTree(); }

            void AbstractManager::setSubRootElement(gui::element::AbstractElement *element) {
                createRootElement();
                addChildToRootElement(element);
            }

            void AbstractManager::deleteElementsTree() { delete elementsTree; }

            void AbstractManager::render(bool clear) {
                if (needUpdate()) {
                    update();
                    needUpdate(false);
                };
                renderElements(clear);
            }
        } // namespace manager
    } // namespace element
} // namespace gui
