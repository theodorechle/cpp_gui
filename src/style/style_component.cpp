#include "style_component.hpp"

void StyleComponent::addStyleDefinition(StyleComponentDataList *componentsList, AppliedStyleMap *styleMap) {
    styleDef->push_back(std::pair<StyleComponentDataList, AppliedStyleMap>(*componentsList, *styleMap));
}
