#include "container.hpp"

namespace gui::element {
    Container::Container(gui::elementStyle::manager::StyleManager *elementsStyleManager, std::vector<std::string> *classes,
                         const std::string &identifier)
        : UiElement{"container", elementsStyleManager, classes, identifier} {}

    void Container::computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                               std::list<std::tuple<int, int>> childsSizes) const {
        std::list<int> childsWidths;
        std::list<int> childsHeights;

        for (const std::tuple<int, int> &childSize : childsSizes) {
            childsWidths.push_back(std::get<0>(childSize));
            childsHeights.push_back(std::get<1>(childSize));
        }

        (*selfWidth) = *std::max_element(childsWidths.cbegin(), childsWidths.cend());
        (*selfHeight) = *std::max_element(childsHeights.cbegin(), childsHeights.cend());
    }

    void Container::renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                 std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {
        const UiElement *elementChild = static_cast<const UiElement *>(child());
        int childNb = 1;
        while (elementChild != nullptr) {
            renderSingleChildWrapper(renderChildCallback, childInfosCallback, elementChild, {0, 0});
            childNb++;
            elementChild = static_cast<const UiElement *>(elementChild->next());
        }
    }
} // namespace gui::element