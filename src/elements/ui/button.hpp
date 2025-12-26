#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "ui_element.hpp"

#include <functional>

namespace gui::element {
    class Button : public UiElement {
    public:
        Button(EventHandler onClick, gui::elementStyle::manager::StyleManager *elementsStyleManager = nullptr,
               std::vector<std::string> *classes = nullptr, const std::string &identifier = "");

        void computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                        std::list<std::tuple<int, int>> childsSizes) const override;

        void renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                          std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const override;
    };
} // namespace gui::element

#endif // BUTTON_HPP
