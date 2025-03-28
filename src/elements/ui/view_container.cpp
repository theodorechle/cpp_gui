#include "view_container.hpp"

namespace gui {
    namespace element {

        void ViewContainer::computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight) {
            if (viewManager == nullptr) return;
            viewManager->computeDesiredElementsLayout(desiredWidth, desiredHeight);
        }

        void ViewContainer::computeChildsLayout(int x, int y, int availableWidth, int availableHeight) {
            if (viewManager == nullptr) return;
            viewManager->setClipRect(SDL_Rect{x, y, availableWidth, availableHeight});
            viewManager->computeFinalElementsLayout();
        }

        void ViewContainer::renderChilds() {
            if (viewManager == nullptr) return;
            std::cerr << "rendering\n";
            viewManager->askRendering();
            viewManager->render(false);
        }
        
        void ViewContainer::catchEvent(const SDL_Event &event) {
            if (viewManager == nullptr) return;
            viewManager->processEvent(event);
            std::cerr << viewManager->needRecomputeLayout() << "\n";
            if (viewManager->needRecomputeLayout()) askRecomputeLayout();
        }

        void ViewContainer::setModifierState(std::string modifierName, bool enabled) {
            AbstractElement::setModifierState(modifierName, enabled);
            viewManager->processMouseEvents();
            askRecomputeLayout();
            viewManager->needRecomputeLayout();
        }

        ViewContainer::ViewContainer(gui::element::manager::UIManager *viewManager,
                                     gui::elementStyle::manager::StyleNodesManager *elementsStyleManager, std::vector<std::string> *classes,
                                     const std::string &identifier)
            : UiElement("view-container", elementsStyleManager, classes, identifier), viewManager{viewManager} {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "WARNING, this element (ViewContainer) does not fully work.");
            }
    } // namespace element
} // namespace gui
