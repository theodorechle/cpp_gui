#ifndef ROOT_ELEMENT_HPP
#define ROOT_ELEMENT_HPP

#include "ui_element.hpp"
#include <numeric>

namespace gui::element {
    class RootElement : public UiElement {
    public:
        RootElement(gui::elementStyle::manager::StyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                    const std::string &identifier = "");
        void computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                        std::list<std::tuple<int, int>> childsSizes) const override;

        void renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                          std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const override;
    };
} // namespace gui::element

#endif // ROOT_ELEMENT_HPP
