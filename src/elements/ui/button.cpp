#include "button.hpp"

namespace gui {
    namespace element {

        Button::Button(std::function<void()> onClick, gui::elementStyle::manager::StyleNodesManager *elementsStyleManager,
                       std::vector<std::string> *classes, const std::string &identifier)
            : UiElement{"button", elementsStyleManager, classes, identifier}, onClickFunction{onClick} {}

        void Button::catchEvent(const SDL_Event &event) {
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && onClickFunction) (onClickFunction)();
        }

        void Button::computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
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

        void Button::renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                  std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {
            const UiElement *child = getConstChild();
            int childNb = 1;
            while (child != nullptr) {
                renderSingleChildWrapper(renderChildCallback, childInfosCallback, child, {0, 0});
                childNb++;
                child = child->getConstNext();
            }
        }
    } // namespace element
} // namespace gui
