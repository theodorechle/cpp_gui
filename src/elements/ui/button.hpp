#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "ui_element.hpp"

#include <functional>

namespace gui {
    namespace element {

        class Button : public UiElement {
            std::function<void()> onClickFunction;

        public:
            Button(std::function<void()> onClick, gui::elementStyle::manager::StyleNodesManager *elementsStyleManager = nullptr,
                   std::vector<std::string> *classes = nullptr, const std::string &identifier = "");

            void catchEvent(const SDL_Event &event) override;
            void computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                            std::list<std::tuple<int, int>> childsSizes) const override;

            void renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                              std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const override;
        };

    } // namespace element
} // namespace gui

#endif // BUTTON_HPP
