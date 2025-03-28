#ifndef ELEMENT_TO_RENDER_NODE_CONVERTER_HPP
#define ELEMENT_TO_RENDER_NODE_CONVERTER_HPP

#include "ui_render_node.hpp"
#include "../ui_element.hpp"

namespace gui::element::ui::render {
    UiRenderNode *elementToRenderNodeConverter(UiRenderNode *parent, UiElement *element);
} // namespace gui::elements::ui::render

#endif // ELEMENT_TO_RENDER_NODE_CONVERTER_HPP
