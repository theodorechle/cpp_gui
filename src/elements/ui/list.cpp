#include "list.hpp"

namespace gui {
    namespace element {
        List::List(gui::elementStyle::manager::StyleNodesManager *elementsStyleManager, std::vector<std::string> *classes,
                   const std::string &identifier)
            : UiElement{"list", elementsStyleManager, classes, identifier} {}

        void List::computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                              std::list<std::tuple<int, int>> childsSizes) const {
            bool vertical = getBoolFromRule({"vertical"});
            int gap;
            std::list<int> childsWidths;
            std::list<int> childsHeights;

            for (const std::tuple<int, int> &childSize : childsSizes) {
                childsWidths.push_back(std::get<0>(childSize));
                childsHeights.push_back(std::get<1>(childSize));
            }

            if (vertical) {
                (*selfWidth) = *std::max_element(childsWidths.cbegin(), childsWidths.cend());
                (*selfHeight) = std::accumulate(childsHeights.cbegin(), childsHeights.cend(), 0);
                gap = computeSize({"gap"}, 0, false, (constParent() == nullptr) ? 0 : constParent()->getHeight()) * (nbChilds() - 1);
                (*selfHeight) += gap * (childsSizes.size() - 1);
            }
            else {
                (*selfHeight) = *std::max_element(childsHeights.cbegin(), childsHeights.cend());
                (*selfWidth) = std::accumulate(childsWidths.cbegin(), childsWidths.cend(), 0);
                gap = computeSize({"gap"}, 0, false, (constParent() == nullptr) ? 0 : constParent()->getWidth()) * (nbChilds() - 1);
                (*selfWidth) += gap * (childsSizes.size() - 1);
            }
        }

        void List::renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {
            bool vertical = getBoolFromRule({"vertical"});
            int gap;
            if (vertical) {
                gap = computeSize({"gap"}, 0, false, (constParent() == nullptr) ? 0 : constParent()->getHeight()) * (nbChilds() - 1);
            }
            else {
                gap = computeSize({"gap"}, 0, false, (constParent() == nullptr) ? 0 : constParent()->getWidth()) * (nbChilds() - 1);
            }
            const ui::UiElementData *childData;
            ui::Pos childCoords = {0, 0};
            const UiElement *child = constChild();
            while (child != nullptr) {
                renderSingleChildWrapper(renderChildCallback, childInfosCallback, child, childCoords);
                childData = static_cast<const ui::UiElementData *>(childInfosCallback(child));
                if (vertical) {
                    childCoords.y += childData->clippedElementSize.height + gap;
                }
                else {
                    childCoords.x += childData->clippedElementSize.width + gap;
                }
                child = child->constNext();
            }
        }
    } // namespace element
} // namespace gui
