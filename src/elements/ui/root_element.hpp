#ifndef ROOT_ELEMENT_HPP
#define ROOT_ELEMENT_HPP

#include "ui_element.hpp"

namespace gui::element {
    // Internal use only
    class RootElement : public UiElement {
    public:
        RootElement(gui::elementStyle::manager::StyleManager *elementsStyleManager = nullptr, std::vector<std::string> classes = {},
                    const std::string &identifier = "");
        void computeSelfAndChildrenLayout(int *selfWidth, int *selfHeight, std::list<std::tuple<int, int>> childrenSizes) const override;

        void renderChildren(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                          std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const override;
    };
} // namespace gui::element

#endif // ROOT_ELEMENT_HPP
