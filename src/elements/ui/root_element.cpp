#include "root_element.hpp"
#include <algorithm>

namespace gui::element {
    RootElement::RootElement(gui::elementStyle::manager::StyleManager *elementsStyleManager, std::vector<std::string> classes,
                             const std::string &identifier)
        : UiElement{"root", elementsStyleManager, classes, identifier} {}

    void RootElement::computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, std::list<std::tuple<int, int>> childsSizes) const {
        std::list<int> childsWidths;
        std::list<int> childsHeights;

        for (const std::tuple<int, int> &childSize : childsSizes) {
            childsWidths.push_back(std::get<0>(childSize));
            childsHeights.push_back(std::get<1>(childSize));
        }

        (*selfWidth) = *std::max_element(childsWidths.cbegin(), childsWidths.cend());
        (*selfHeight) = *std::max_element(childsHeights.cbegin(), childsHeights.cend());
    }

    void RootElement::renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                   std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {
        const UiElement *elementChild = static_cast<const UiElement *>(child());
        while (elementChild != nullptr) {
            renderSingleChildWrapper(renderChildCallback, childInfosCallback, elementChild, {0, 0});
            elementChild = static_cast<const UiElement *>(elementChild->next());
        }
    }
} // namespace gui::element
