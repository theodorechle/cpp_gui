#include "button.hpp"
#include <algorithm>

namespace gui::element {
    Button::Button(EventHandler onClick, gui::elementStyle::manager::StyleManager *elementsStyleManager, std::vector<std::string> classes,
                   const std::string &identifier)
        : UiElement{"button", elementsStyleManager, classes, identifier} {
        if (onClick != nullptr) registerEventHandler(ui::event::GuiEventType::EVENT_MOUSE_BUTTON_DOWN, onClick);
    }

    void Button::computeSelfAndChildrenLayout(int *selfWidth, int *selfHeight, std::list<std::tuple<int, int>> childrenSizes) const {
        std::list<int> childrenWidths;
        std::list<int> childrenHeights;

        for (const std::tuple<int, int> &childSize : childrenSizes) {
            childrenWidths.push_back(std::get<0>(childSize));
            childrenHeights.push_back(std::get<1>(childSize));
        }

        (*selfWidth) = *std::max_element(childrenWidths.cbegin(), childrenWidths.cend());
        (*selfHeight) = *std::max_element(childrenHeights.cbegin(), childrenHeights.cend());
    }

    void Button::renderChildren(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                              std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {
        const UiElement *elementChild = static_cast<const UiElement *>(child());
        while (elementChild != nullptr) {
            renderSingleChildWrapper(renderChildCallback, childInfosCallback, elementChild, {0, 0});
            elementChild = static_cast<const UiElement *>(elementChild->next());
        }
    }
} // namespace gui::element
