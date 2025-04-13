#ifndef VIEW_CONTAINER_HPP
#define VIEW_CONTAINER_HPP

#include "ui_element.hpp"
#include "ui_manager.hpp"

namespace gui {
    namespace element {

        class ViewContainer : public UiElement {
            gui::element::manager::UIManager *viewManager = nullptr;

            void computeSelfLayout(int *width, int *height) const override;
            void computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, std::list<std::tuple<int, int>> childsSizes) const override;

            void renderChilds() const override;
            virtual void catchEvent(const SDL_Event &event) override;

            /**
             * true if should propagate to the parent, false else
             */
            void setModifierState(std::string modifierName, bool enabled);

        public:
            ViewContainer(gui::element::manager::UIManager *viewManager,
                          gui::elementStyle::manager::StyleNodesManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                          const std::string &identifier = "");
        };

    } // namespace element
} // namespace gui

#endif // VIEW_CONTAINER_HPP
