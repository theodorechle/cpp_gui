#ifndef VIEW_CONTAINER_HPP
#define VIEW_CONTAINER_HPP

#include "managers/ui_manager.hpp"
#include "ui_element.hpp"

namespace gui {
    namespace element {

        class ViewContainer : public UIElement {
            gui::element::manager::UIManager *viewManager = nullptr;

            void computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight) override;
            void computeChildsLayout(int x, int y, int availableWidth, int availableHeight) override;

            void renderChilds() override;
            virtual void catchEvent(const SDL_Event &event) override;

            /**
             * true if should propagate to the parent, false else
             */
            void setModifierState(std::string modifierName, bool enabled);

        public:
            ViewContainer(gui::element::manager::UIManager *viewManager,
                          gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr,
                          std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
        };

    } // namespace element
} // namespace gui

#endif // VIEW_CONTAINER_HPP
