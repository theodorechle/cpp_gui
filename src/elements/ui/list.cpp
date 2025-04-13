#include "list.hpp"

namespace gui {
    namespace element {
        void List::computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, std::list<std::tuple<int, int>> childsSizes) const {
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
                std::accumulate(childsWidths.cbegin(), childsWidths.cend(), 0);
            }
            else {
                (*selfHeight) = *std::max_element(childsHeights.cbegin(), childsHeights.cend());
                std::accumulate(childsHeights.cbegin(), childsHeights.cend(), 0);
            }

            if (childsLayout == "biggest") {
                if (vertical) {
                    (*selfHeight) = *std::max_element(childsHeights.cbegin(), childsHeights.cend());
                }
                else {
                    (*selfWidth) = *std::max_element(childsWidths.cbegin(), childsWidths.cend());
                }
            }
            else if (childsLayout == "keep") {
                for (const std::tuple<int, int> &childSize : childsSizes) {
                    if (vertical) (*selfHeight) += std::get<1>(childSize);
                    else (*selfWidth) += std::get<0>(childSize);
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

        List::List(gui::elementStyle::manager::StyleNodesManager *elementsStyleManager, std::vector<std::string> *classes,
                   const std::string &identifier)
            : UiElement{"list", elementsStyleManager, classes, identifier} {}
    } // namespace element
} // namespace gui
