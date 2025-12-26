#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "ui_element.hpp"

namespace gui::element {
    class Container : public UiElement {
    public:
        Container(gui::elementStyle::manager::StyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                  const std::string &identifier = "");
        void computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                        std::list<std::tuple<int, int>> childsSizes) const override;

        void renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                          std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const override;
    };
} // namespace gui::element

#endif // CONTAINER_HPP
