#include "ui_element.hpp"

namespace gui {
    namespace element {
        SDL_Rect UiElement::computeNewClipRect(SDL_Rect *oldClipRect, SDL_Rect *wantedNewClipRect) {
            return SDL_Rect{std::max(oldClipRect->x, wantedNewClipRect->x), std::max(oldClipRect->y, wantedNewClipRect->y),
                            std::min(oldClipRect->w, wantedNewClipRect->w), std::min(oldClipRect->h, wantedNewClipRect->h)};
        }

        SDL_FRect UiElement::createFRect(int x, int y, int width, int height) { return SDL_FRect{(float)x, (float)y, (float)width, (float)height}; }

        int UiElement::getIntFromRule(const std::vector<std::string> &ruleNames, int defaultSize, bool canInherit) const {
            if (style == nullptr) return defaultSize;
            style::StyleValue *rule = nullptr;
            int size = 0;
            style->getRule(ruleNames, &rule, canInherit);
            if (rule == nullptr) {
                return defaultSize;
            }
            if (!converter::NumberConverter::convertToInt(rule, &size)) {
                return defaultSize;
            }
            return size;
        }

        std::string UiElement::getStringFromRule(const std::vector<std::string> &ruleNames, const std::string &defaultString, bool canInherit) const {
            if (style == nullptr) return defaultString;
            style::StyleValue *rule = nullptr;
            style->getRule(ruleNames, &rule, canInherit);
            if (rule == nullptr || rule->getType() != style::StyleValueType::String) {
                return defaultString;
            }
            return rule->getValue();
        }

        std::string UiElement::getNameStringFromRule(const std::string &ruleName, const std::vector<std::string> &allowedValues,
                                                     const std::string &defaultString, bool canInherit) const {
            if (style == nullptr) return defaultString;
            style::StyleValue *rule = nullptr;
            style->getRule(ruleName, &rule, canInherit);
            if (rule
                == nullptr
                || rule->getType()
                != style::StyleValueType::NameString
                || allowedValues.empty()
                || std::find(allowedValues.cbegin(), allowedValues.cend(), rule->getValue())
                == allowedValues.cend()) {
                return defaultString;
            }
            return rule->getValue();
        }

        std::string UiElement::getNameStringFromRules(const std::vector<std::string> &ruleNames, const std::vector<std::string> &allowedValues,
                                                      const std::string &defaultString, bool canInherit) const {
            if (style == nullptr) return defaultString;
            style::StyleValue *rule = nullptr;
            style->getRule(ruleNames, &rule, canInherit);
            if (rule
                == nullptr
                || rule->getType()
                != style::StyleValueType::NameString
                || allowedValues.empty()
                || std::find(allowedValues.cbegin(), allowedValues.cend(), rule->getValue())
                == allowedValues.cend()) {
                return defaultString;
            }
            return rule->getValue();
        }

        bool UiElement::getBoolFromRule(const std::vector<std::string> &ruleNames, bool defaultBool, bool canInherit) const {
            bool value;
            if (style == nullptr) return defaultBool;
            style::StyleValue *rule = nullptr;
            style->getRule(ruleNames, &rule, canInherit);
            if (converter::BoolConverter::convert(rule, &value)) return value;
            return defaultBool;
        }

        int UiElement::computeSize(const std::vector<std::string> &ruleNames, int defaultSize, bool canInherit, int parentSize, bool *found) const {
            if (style == nullptr) return defaultSize;
            style::StyleValue *rule = nullptr;
            int size = 0;
            style->getRule(ruleNames, &rule, canInherit);
            if (found != nullptr) (*found) = (rule != nullptr);
            if (rule == nullptr) {
                return defaultSize;
            }
            if (!converter::SizeConverter::convert(rule, &size, parentSize)) {
                return defaultSize;
            }
            return size;
        }

        SDL_Color UiElement::computeColor(const std::vector<std::string> &ruleNames, SDL_Color defaultColor, bool canInherit) const {
            if (style == nullptr) return defaultColor;
            style::StyleValue *rule = nullptr;
            SDL_Color color = SDL_Color();
            style->getRule(ruleNames, &rule, canInherit);
            if (rule == nullptr) {
                return defaultColor;
            }
            if (!converter::ColorConverter::convert(rule, &color)) {
                return defaultColor;
            }
            return color;
        }

        void UiElement::addChild(UiElement *child) {
            if (child == nullptr) return;
            AbstractElement::addChild(child);
            child->setRenderer(renderer);
            child->setWindow(window);
            updated();
        }

        void UiElement::setWindow(SDL_Window *window) {
            this->window = window;
            UiElement *c = child();
            while (c != nullptr) {
                c->setWindow(window);
                c = c->next();
            }
        }

        void UiElement::setRenderer(SDL_Renderer *renderer) {
            this->renderer = renderer;
            UiElement *c = child();
            while (c != nullptr) {
                c->setRenderer(renderer);
                c = c->next();
            }
        }

        int UiElement::marginLeft(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"margin-left", "margin"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        }

        int UiElement::marginRight(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"margin-right", "margin"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        }

        int UiElement::marginTop(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"margin-top", "margin"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        }

        int UiElement::marginBottom(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"margin-bottom", "margin"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        }

        int UiElement::paddingLeft(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"padding-left", "padding"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        }

        int UiElement::paddingRight(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"padding-right", "padding"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        }

        int UiElement::paddingTop(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"padding-top", "padding"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        }

        int UiElement::paddingBottom(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"padding-bottom", "padding"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        }

        int UiElement::borderLeft(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"border-left", "border"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        }

        int UiElement::borderRight(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"border-right", "border"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        }

        int UiElement::borderTop(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"border-top", "border"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        }

        int UiElement::borderBottom(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"border-bottom", "border"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        }

        int UiElement::width(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"width"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        };

        int UiElement::height(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"height"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        };

        int UiElement::maxWidth(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"max-width"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        };

        int UiElement::minWidth(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"min-width"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        };

        int UiElement::maxHeight(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"max-height"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        };

        int UiElement::minHeight(bool *found) const {
            const UiElement *parent = constParent();
            return computeSize({"min-height"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        };

        SDL_Color UiElement::borderLeftColor() const { return computeColor({"border-left-color", "border-color"}); }

        SDL_Color UiElement::borderRightColor() const { return computeColor({"border-right-color", "border-color"}); }

        SDL_Color UiElement::borderTopColor() const { return computeColor({"border-top-color", "border-color"}); }

        SDL_Color UiElement::borderBottomColor() const { return computeColor({"border-bottom-color", "border-color"}); }

        SDL_Color UiElement::backgroundColor() const { return computeColor({"background-color"}, SDL_Color{255, 255, 255, 0}); }

        void UiElement::computeTotalLayout(int *width, int *height) const {
            bool widthFound = false;
            bool heightFound = false;
            int tempWidth = this->width(&widthFound);
            int tempHeight = this->height(&heightFound);
            if (widthFound) {
                (*width) = tempWidth;
            }
            else {
                (*width) += paddingLeft() + paddingRight() + borderLeft() + borderRight();
            }

            if (heightFound) {
                (*height) = tempHeight;
            }
            else {
                (*height) += paddingTop() + paddingBottom() + borderTop() + borderBottom();
            }

            bool found = false;
            int size;
            size = minWidth(&found);
            if (found) (*width) = std::max(*width, size);
            size = maxWidth(&found);
            if (found) (*width) = std::min(*width, size);
            size = minHeight(&found);
            if (found) (*height) = std::max(*height, size);
            size = maxHeight(&found);
            if (found) (*height) = std::min(*height, size);
        }

        void UiElement::computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                                   std::list<std::tuple<int, int>> childsSizes) const {
            // (*selfWidth) = (*selfWidthWithoutChilds);
            // (*selfHeight) = (*selfHeightWithoutChilds);
            computeInnerLayout(selfWidth, selfHeight);
        }

        bool UiElement::setClipRect(const SDL_Rect *clipRect, std::string callerName) const {
            if (!SDL_SetRenderClipRect(renderer, clipRect)) {
                SDL_LogError(ui::GUI_RENDERING, "%s: can't set clip rect '%s'", callerName.c_str(), SDL_GetError());
                return false;
            }
            return true;
        }

        void UiElement::renderSelfBeforeChilds() const {}

        void UiElement::renderSelfAfterChilds() const {}

        void UiElement::renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                     std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {}

        void UiElement::computeInnerLayout(int *width, int *height) const {}

        bool UiElement::render(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                               std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {
            if (renderer == nullptr) throw NoRendererException(); // TODO: exception or simple error log? (coherence with the entire program)

            SDL_Rect clipRect;
            SDL_GetRenderClipRect(renderer, &clipRect);

            renderBordersWrapper();

            SDL_Rect innerRect = {clipRect.x + borderLeft(), clipRect.y + borderTop(), clipRect.w - borderLeft() - borderRight(),
                                  clipRect.h - borderTop() - borderBottom()};

            if (!setClipRect(&innerRect, "UiElement::render (inner rect)")) return false;

            renderBackgroundWrapper();

            innerRect.x += paddingLeft();
            innerRect.y += paddingTop();
            innerRect.w -= paddingLeft() + paddingRight();
            innerRect.h -= paddingTop() + paddingBottom();

            if (!setClipRect(&innerRect, "UiElement::render (inner rect)")) return false;

            renderSelfBeforeChildsWrapper();

            renderChildsWrapper(renderChildCallback, childInfosCallback);

            renderSelfAfterChildsWrapper();
            if (!setClipRect(&clipRect, "UiElement::render (restore)")) return false;

            renderScrollBarsWrapper();

            return true;
        }

        void UiElement::renderBorders() const {
            SDL_Color color;
            Uint8 r, g, b, a;
            SDL_FRect fRect;
            SDL_Rect clipRect;
            int bLeft = borderLeft();
            int bRight = borderRight();
            int bTop = borderTop();
            int bBottom = borderBottom();

            if (!SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UiElement::renderBorders: Can't get draw color '%s'", SDL_GetError());
                return;
            }
            SDL_GetRenderClipRect(renderer, &clipRect);

            // left border
            color = borderLeftColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UiElement::renderBorders: Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(clipRect.x, clipRect.y + bTop, bLeft, clipRect.h - bTop - bBottom);
            SDL_RenderFillRect(renderer, &fRect);

            // right border
            color = borderRightColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UiElement::renderBorders: Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(clipRect.x + clipRect.w, clipRect.y + bTop, -bRight, clipRect.h - bTop - bBottom);
            SDL_RenderFillRect(renderer, &fRect);

            // top border
            color = borderTopColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UiElement::renderBorders: Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(clipRect.x, clipRect.y, clipRect.w, bTop);
            SDL_RenderFillRect(renderer, &fRect);

            // bottom border
            color = borderBottomColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UiElement::renderBorders: Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(clipRect.x, clipRect.y + clipRect.h, clipRect.w, -bBottom);
            SDL_RenderFillRect(renderer, &fRect);

            // restore previous color
            if (!SDL_SetRenderDrawColor(renderer, r, g, b, a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UiElement::renderBorders: Can't set draw color '%s'", SDL_GetError());
                return;
            }
        }

        void UiElement::renderBackground() const {
            Uint8 r, g, b, a;
            SDL_Rect rect;
            SDL_FRect fRect;

            if (!SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UiElement::renderBackground: Can't get draw color '%s'", SDL_GetError());
                return;
            }

            SDL_Color color = backgroundColor();
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_GetRenderClipRect(renderer, &rect);

            SDL_RectToFRect(&rect, &fRect);
            SDL_RenderFillRect(renderer, &fRect);

            if (!SDL_SetRenderDrawColor(renderer, r, g, b, a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UiElement::renderBackground: Can't set draw color '%s'", SDL_GetError());
                return;
            }
        }

        void UiElement::renderScrollBar(int currentSize, int desiredSize) const {
            if (currentSize >= desiredSize && getNameStringFromRule("scroll-bar", {"always"}) == "") return;
        }

        void UiElement::renderScrollBars() const {
            // TODO: do it
            // renderScrollBar();
            // renderScrollBar();
        }

        void UiElement::renderSelfBeforeChildsWrapper() const { renderSelfBeforeChilds(); }

        void UiElement::renderSelfAfterChildsWrapper() const { renderSelfAfterChilds(); }

        void UiElement::renderChildsWrapper(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                            std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {
            renderChilds(renderChildCallback, childInfosCallback);
        }

        bool UiElement::renderSingleChildWrapper(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                                 std::function<const ElementData *(const AbstractElement *)> childInfosCallback,
                                                 const UiElement *child, ui::Pos pos) const {

            const ui::UiElementData *childData = static_cast<const ui::UiElementData *>(childInfosCallback(child));

            // TODO: cleanup
            SDL_Rect childRect = SDL_Rect{pos.x, pos.y, childData->elementSize.width, childData->elementSize.height};
            childRect.x += paddingLeft() + borderLeft();
            childRect.y += paddingTop() + borderTop();
            delete childData;
            SDL_Rect selfRect;
            SDL_GetRenderClipRect(renderer, &selfRect);
            ui::UiRenderData data = ui::UiRenderData(computeNewClipRect(&selfRect, &childRect));
            bool childRendered = renderChildCallback(child, &data);
            return childRendered;
        }

        void UiElement::renderBackgroundWrapper() const { renderBackground(); }

        void UiElement::renderBordersWrapper() const { renderBorders(); }

        void UiElement::renderScrollBarWrapper(int currentSize, int desiredSize) const { renderScrollBar(currentSize, desiredSize); }

        void UiElement::renderScrollBarsWrapper() const { renderScrollBars(); }

        void UiElement::focus(bool focused) {
            _focus = focused;
            if (focused) onFocusGet();
            else onFocusLoose();
        }
    } // namespace element
} // namespace gui
