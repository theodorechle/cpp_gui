#include "list.hpp"
#include <algorithm>
#include <numeric>

namespace gui::element {
    List::List(gui::elementStyle::manager::StyleManager *elementsStyleManager, std::vector<std::string> classes, const std::string &identifier)
        : UiElement{"list", elementsStyleManager, classes, identifier} {}

    void List::computeSelfAndChildrenLayout(int *selfWidth, int *selfHeight, std::list<std::tuple<int, int>> childrenSizes) const {
        bool vertical = getBoolFromRule({"vertical"});
        int gap;
        std::list<int> childrenWidths;
        std::list<int> childrenHeights;

        for (const std::tuple<int, int> &childSize : childrenSizes) {
            childrenWidths.push_back(std::get<0>(childSize));
            childrenHeights.push_back(std::get<1>(childSize));
        }

        if (vertical) {
            (*selfWidth) = *std::max_element(childrenWidths.cbegin(), childrenWidths.cend());
            (*selfHeight) = std::accumulate(childrenHeights.cbegin(), childrenHeights.cend(), 0);
            gap = computeSize({"gap"}, 0, (parent() == nullptr) ? 0 : static_cast<const UiElement *>(parent())->getHeight()) * (nbChildren() - 1);
            (*selfHeight) += gap * (childrenSizes.size() - 1);
        }
        else {
            (*selfHeight) = *std::max_element(childrenHeights.cbegin(), childrenHeights.cend());
            (*selfWidth) = std::accumulate(childrenWidths.cbegin(), childrenWidths.cend(), 0);
            gap = computeSize({"gap"}, 0, (parent() == nullptr) ? 0 : static_cast<const UiElement *>(parent())->getWidth()) * (nbChildren() - 1);
            (*selfWidth) += gap * (childrenSizes.size() - 1);
        }
    }

    void List::renderChildren(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                            std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {
        bool vertical = getBoolFromRule({"vertical"});
        int gap;
        if (vertical) {
            gap = computeSize({"gap"}, 0, (parent() == nullptr) ? 0 : static_cast<const UiElement *>(parent())->getHeight()) * (nbChildren() - 1);
        }
        else {
            gap = computeSize({"gap"}, 0, (parent() == nullptr) ? 0 : static_cast<const UiElement *>(parent())->getWidth()) * (nbChildren() - 1);
        }
        const ui::UiElementData *childData;
        ui::Pos childCoords = {0, 0};
        const UiElement *elementChild = static_cast<const UiElement *>(child());
        while (elementChild != nullptr) {
            renderSingleChildWrapper(renderChildCallback, childInfosCallback, elementChild, childCoords);
            childData = static_cast<const ui::UiElementData *>(childInfosCallback(elementChild));
            if (vertical) {
                childCoords.y += childData->clippedElementSize.height + gap;
            }
            else {
                childCoords.x += childData->clippedElementSize.width + gap;
            }
            delete childData;
            elementChild = static_cast<const UiElement *>(elementChild->next());
        }
    }
} // namespace gui::element
