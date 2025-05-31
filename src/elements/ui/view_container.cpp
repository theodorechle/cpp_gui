#include "view_container.hpp"

namespace gui {
    namespace element {

        void ViewContainer::computeTotalLayout(int *width, int *height) const {
            if (viewManager == nullptr) return;
            // TODO: re-add
            // viewManager->computeDesiredElementsLayout(desiredWidth, desiredHeight);
        }

        void ViewContainer::computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                                       std::list<std::tuple<int, int>> childsSizes) const {
            if (viewManager == nullptr) return;
            // TODO: re-add
            // viewManager->setClipRect(SDL_Rect{x, y, availableWidth, availableHeight});
            // viewManager->computeFinalElementsLayout();
        }

        void ViewContainer::renderChildsWrapper(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                                std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {
            if (viewManager == nullptr) return;
            std::cerr << "rendering\n";
            // TODO: re-add
            // viewManager->askRendering();
            // viewManager->render(false);
        }

        void ViewContainer::catchEvent(const SDL_Event &event) {
            if (viewManager == nullptr) return;
            // TODO: re-add
            // viewManager->processEvent(event);
            // std::cerr << viewManager->needRecomputeLayout() << "\n";
            // if (viewManager->needRecomputeLayout()) askRecomputeLayout();
        }

        void ViewContainer::setModifierState(std::string modifierName, bool enabled) {
            // TODO: re-add
            // AbstractElement::setModifierState(modifierName, enabled);
            // viewManager->processMouseEvents();
            // askRecomputeLayout();
            // viewManager->needRecomputeLayout();
        }

        ViewContainer::ViewContainer(gui::element::manager::UIManager *viewManager,
                                     gui::elementStyle::manager::StyleNodesManager *elementsStyleManager, std::vector<std::string> *classes,
                                     const std::string &identifier)
            : UiElement("view-container", elementsStyleManager, classes, identifier), viewManager{viewManager} {
            SDL_LogError(ui::GUI_RENDERING, "WARNING, this element (ViewContainer) does not fully work.");
        }
    } // namespace element
} // namespace gui
