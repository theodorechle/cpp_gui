#include "element_to_render_node_converter.hpp"

namespace gui::element::ui::render {
    UiRenderNode *elementToRenderNodeConverter(SDL_Renderer *renderer, UiRenderNode *parent, UiElement *element) { return new UiRenderNode(renderer, parent, element); }
} // namespace gui::elements::ui::render
