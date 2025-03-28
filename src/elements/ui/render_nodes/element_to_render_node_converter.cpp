#include "element_to_render_node_converter.hpp"

namespace gui::element::ui::render {
    UiRenderNode *elementToRenderNodeConverter(UiRenderNode *parent, UiElement *element) { return new UiRenderNode(parent, element); }
} // namespace gui::elements::ui::render
