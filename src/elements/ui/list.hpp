#ifndef LIST_HPP
#define LIST_HPP

#include "ui_element.hpp"
#include <numeric>

namespace gui::element {
    class List : public UiElement {
    public:
        List(gui::elementStyle::manager::StyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
             const std::string &identifier = "");
        void computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                        std::list<std::tuple<int, int>> childsSizes) const override;
        void renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                          std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const override;
    };
} // namespace gui::element

#endif // LIST_HPP
