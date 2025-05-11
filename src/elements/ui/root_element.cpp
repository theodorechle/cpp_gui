#include "root_element.hpp"

namespace gui {
    namespace element {
        RootElement::RootElement(gui::elementStyle::manager::StyleNodesManager *elementsStyleManager, std::vector<std::string> *classes,
                                 const std::string &identifier)
            : UiElement{"root", elementsStyleManager, classes, identifier} {}

        void RootElement::computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                                     std::list<std::tuple<int, int>> childsSizes) const {
            std::list<int> childsWidths;
            std::list<int> childsHeights;

            for (const std::tuple<int, int> &childSize : childsSizes) {
                childsWidths.push_back(std::get<0>(childSize));
                childsHeights.push_back(std::get<1>(childSize));
            }

            (*selfWidth) = *std::max_element(childsWidths.cbegin(), childsWidths.cend());
            (*selfHeight) = std::accumulate(childsWidths.cbegin(), childsWidths.cend(), 0);
        }

        void RootElement::renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                       std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {
            const UiElement *child = getConstChild();
            int childNb = 1;
            while (child != nullptr) {
                std::cerr << "child (" << childNb << " of " << nbChilds() << ") of '" << name() << "': " << child->name() << "\n";
                renderSingleChildWrapper(renderChildCallback, childInfosCallback, child, {0, 0});
                child = child->getConstNext();
            }
        }
    } // namespace element
} // namespace gui
