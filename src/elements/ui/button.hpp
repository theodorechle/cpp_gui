#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "ui_element.hpp"

#include <functional>

namespace gui::element {
    class Button : public UiElement {
    public:
        Button(EventHandler onClick, gui::elementStyle::manager::StyleManager *elementsStyleManager = nullptr, std::vector<std::string> classes = {},
               const std::string &identifier = "");

        void computeSelfAndChildrenLayout(int *selfWidth, int *selfHeight, std::list<std::tuple<int, int>> childrenSizes) const override;

        void renderChildren(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                          std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const override;
    };
} // namespace gui::element

#endif // BUTTON_HPP
