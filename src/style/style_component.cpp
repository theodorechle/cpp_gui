#include "style_component.hpp"

void StyleComponent::addStyleDefinition(RequiredStyleComponentsList componentsList, AppliedStyleMap styleMap) {
    styleDef.push_back(std::pair<RequiredStyleComponentsList, AppliedStyleMap>(componentsList, styleMap));
}

const StyleDefinition& StyleComponent::getStyleDefinition() const {
    return styleDef;
}