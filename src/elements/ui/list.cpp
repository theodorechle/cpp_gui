#include "list.hpp"

namespace gui {
    namespace element {
        List::List(gui::elementStyle::manager::StyleNodesManager *elementsStyleManager, std::vector<std::string> *classes,
                   const std::string &identifier)
            : UiElement{"list", elementsStyleManager, classes, identifier} {}

        void List::computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                              std::list<std::tuple<int, int>> childsSizes) const {
            bool vertical = getBoolFromRule({"vertical"});
            std::string childsLayout = getNameStringFromRule("childs-size", {"biggest", "keep"}, "biggest");
            int gap;
            std::list<int> childsWidths;
            std::list<int> childsHeights;

            for (const std::tuple<int, int> &childSize : childsSizes) {
                childsWidths.push_back(std::get<0>(childSize));
                childsHeights.push_back(std::get<1>(childSize));
            }

            if (vertical) {
                (*selfWidth) = *std::max_element(childsWidths.cbegin(), childsWidths.cend());
            }
            else {
                (*selfHeight) = *std::max_element(childsHeights.cbegin(), childsHeights.cend());
            }

            if (childsLayout == "biggest") {
                if (vertical) {
                    (*selfHeight) = *std::max_element(childsHeights.cbegin(), childsHeights.cend()) * nbChilds();
                }
                else {
                    (*selfWidth) = *std::max_element(childsWidths.cbegin(), childsWidths.cend()) * nbChilds();
                }
            }
            else if (childsLayout == "keep") {
                if (vertical) {
                    (*selfHeight) = std::accumulate(childsHeights.cbegin(), childsHeights.cend(), 0);
                }
                else {
                    (*selfWidth) = std::accumulate(childsWidths.cbegin(), childsWidths.cend(), 0);
                }
            }

            if (vertical) {
                gap = computeSize({"gap"}, 0, false, (getConstParent() == nullptr) ? 0 : getConstParent()->getHeight()) * (nbChilds() - 1);
                (*selfHeight) += gap * (childsSizes.size() - 1);
            }
            else {
                gap = computeSize({"gap"}, 0, false, (getConstParent() == nullptr) ? 0 : getConstParent()->getWidth()) * (nbChilds() - 1);
                (*selfWidth) += gap * (childsSizes.size() - 1);
            }
        }

        void List::renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {
            const UiElement *child = getConstChild();
            int childNb = 1;
            while (child != nullptr) {
                std::cerr << "child (" << childNb << " of " << nbChilds() << ") of '" << name() << "': " << child->name() << "\n";
                renderSingleChildWrapper(renderChildCallback, childInfosCallback, child, {0, 0});
                childNb++;
                child = child->getConstNext();
            }
        }
    } // namespace element
} // namespace gui
