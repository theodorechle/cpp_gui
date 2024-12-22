#include "style_component.hpp"

StyleComponent::StyleComponent(StyleComponentDataList *componentsList, StyleValuesMap *styleMap) {
    styleDef = new StyleDefinition(*componentsList, *styleMap);
}
