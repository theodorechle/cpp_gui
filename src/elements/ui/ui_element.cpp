#include "ui_element.hpp"

namespace gui {
    namespace element {

        void UiElement::computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight) {
            int nbChilds = 0;
            (*desiredWidth) = 0;
            (*desiredHeight) = 0;
            int childDesiredWidth = 0;
            int childDesiredHeight = 0;
            UiElement *child = getChild();
            while (child != nullptr) {
                child->computeDesiredLayout(&childDesiredWidth, &childDesiredHeight);
                (*desiredWidth) = std::max(childDesiredWidth, *desiredWidth);
                (*desiredHeight) = std::max(childDesiredHeight, *desiredHeight);
                nbChilds++;
                child = child->getNext();
            }
        }

        void UiElement::computeDesiredLayout(int *desiredWidth, int *desiredHeight) {
            (*desiredWidth) = 0;
            (*desiredHeight) = 0;
            int childDesiredWidth = 0;
            int childDesiredHeight = 0;
            int elementWidth = 0;
            int elementHeight = 0;
            bool widthFound = false;
            bool heightFound = false;
            elementWidth = width(&widthFound);
            elementHeight = height(&heightFound);

            UiElement *child = getChild();
            while (child != nullptr) {
                child->initBeforeLayoutComputing();
                child = child->getNext();
            }

            computeDesiredInnerLayout(desiredWidth, desiredHeight);

            if (widthFound) (*desiredWidth) = elementWidth;
            if (heightFound) (*desiredHeight) = elementHeight;

            if (!widthFound) {
                (*desiredWidth) += paddingLeft() + paddingRight();
                (*desiredWidth) += borderLeft() + borderRight();
            }
            if (!heightFound) {
                (*desiredHeight) += paddingTop() + paddingBottom();
                (*desiredHeight) += borderTop() + borderBottom();
            }

            if (marginsActive && (!heightFound || !widthFound)) {
                UiElement *child = getChild();
                while (child != nullptr) {
                    if (!widthFound) childDesiredWidth = child->marginLeft() + child->marginRight();
                    if (!heightFound) childDesiredHeight = child->marginTop() + child->marginBottom();
                    if (!widthFound) (*desiredWidth) += childDesiredWidth;
                    if (!heightFound) (*desiredHeight) += childDesiredHeight;
                    child = child->getNext();
                }
            }
            bool found = false;
            int size;
            size = minWidth(&found);
            if (found) (*desiredWidth) = std::max(*desiredWidth, size);
            size = maxWidth(&found);
            if (found) (*desiredWidth) = std::min(*desiredWidth, size);
            size = minHeight(&found);
            if (found) (*desiredHeight) = std::max(*desiredHeight, size);
            size = maxHeight(&found);
            if (found) (*desiredHeight) = std::min(*desiredHeight, size);

            setDesiredSize(*desiredWidth, *desiredHeight);
        }

        void UiElement::computeLayout(int x, int y, int availableWidth, int availableHeight) {
            SDL_Rect newRect = SDL_Rect{x, y, availableWidth, availableHeight};
            setRect(newRect);
            fullSize.width = availableWidth;
            fullSize.height = availableHeight;
            x += borderLeft() + marginLeft();
            y += borderTop() + marginTop();
            availableWidth -= borderLeft() + borderRight();
            availableHeight -= borderTop() + borderBottom();
            if (getChild() != nullptr) computeChildsLayout(x, y, availableWidth, availableHeight);
        }

        void UiElement::computeChildsLayout(int x, int y, int availableWidth, int availableHeight) {
            int childWidth = 0;
            int childHeight = 0;
            UiElement *child = getChild();
            while (child != nullptr) {
                child->getDesiredSize(&childWidth, &childHeight);
                child->computeLayout(x, y, std::min(childWidth, availableWidth), std::min(childHeight, availableHeight));
                child = child->getNext();
            }
        }

        void UiElement::setRect(const SDL_Rect &rect) { elementRect = rect; }

        void UiElement::setPos(int x, int y) {
            elementRect.x = x;
            elementRect.y = y;
        }

        void UiElement::getRect(SDL_Rect *rect) const { *rect = this->elementRect; }

        void UiElement::getPos(int *x, int *y) const {
            *x = elementRect.x;
            *y = elementRect.y;
        }

        void UiElement::getSize(int *width, int *height) const {
            *width = this->elementRect.w;
            *height = this->elementRect.h;
        }

        void UiElement::getDesiredSize(int *width, int *height) const {
            *width = elementDesiredSize.width;
            *height = elementDesiredSize.height;
        }

        void UiElement::setSize(int width, int height) {
            this->elementRect.w = width;
            this->elementRect.h = height;
        }

        void UiElement::setDesiredSize(int width, int height) {
            this->elementDesiredSize.width = width;
            this->elementDesiredSize.height = height;
        }

        SDL_Rect UiElement::computeNewClipRect(SDL_Rect *oldClipRect, SDL_Rect *wantedNewClipRect) {
            return SDL_Rect{std::max(oldClipRect->x, wantedNewClipRect->x), std::max(oldClipRect->y, wantedNewClipRect->y),
                            std::min(oldClipRect->w, wantedNewClipRect->w), std::min(oldClipRect->h, wantedNewClipRect->h)};
        }

        SDL_FRect UiElement::createFRect(int x, int y, int width, int height) { return SDL_FRect{(float)x, (float)y, (float)width, (float)height}; }

        int UiElement::getIntFromRule(const std::vector<std::string> &ruleNames, int defaultSize, bool canInherit) const {
            if (elementStyle == nullptr) return defaultSize;
            style::StyleValue *rule = nullptr;
            int size = 0;
            elementStyle->getRule(ruleNames, &rule, canInherit);
            if (rule == nullptr) {
                return defaultSize;
            }
            if (!converter::NumberConverter::convertToInt(rule, &size)) {
                return defaultSize;
            }
            return size;
        }

        std::string UiElement::getStringFromRule(const std::vector<std::string> &ruleNames, const std::string &defaultString, bool canInherit) const {
            if (elementStyle == nullptr) return defaultString;
            style::StyleValue *rule = nullptr;
            elementStyle->getRule(ruleNames, &rule, canInherit);
            if (rule == nullptr || rule->getType() != style::StyleValueType::String) {
                return defaultString;
            }
            return rule->getValue();
        }

        std::string UiElement::getNameStringFromRule(const std::string &ruleName, const std::vector<std::string> &allowedValues,
                                                     const std::string &defaultString, bool canInherit) const {
            if (elementStyle == nullptr) return defaultString;
            style::StyleValue *rule = nullptr;
            elementStyle->getRule(ruleName, &rule, canInherit);
            if (rule == nullptr || rule->getType() != style::StyleValueType::NameString || allowedValues.empty()
                || std::find(allowedValues.cbegin(), allowedValues.cend(), rule->getValue()) == allowedValues.cend()) {
                return defaultString;
            }
            return rule->getValue();
        }

        std::string UiElement::getNameStringFromRules(const std::vector<std::string> &ruleNames, const std::vector<std::string> &allowedValues,
                                                      const std::string &defaultString, bool canInherit) const {
            if (elementStyle == nullptr) return defaultString;
            style::StyleValue *rule = nullptr;
            elementStyle->getRule(ruleNames, &rule, canInherit);
            if (rule == nullptr || rule->getType() != style::StyleValueType::NameString || allowedValues.empty()
                || std::find(allowedValues.cbegin(), allowedValues.cend(), rule->getValue()) == allowedValues.cend()) {
                return defaultString;
            }
            return rule->getValue();
        }

        bool UiElement::getBoolFromRule(const std::vector<std::string> &ruleNames, bool defaultBool, bool canInherit) const {
            bool value;
            if (elementStyle == nullptr) return defaultBool;
            style::StyleValue *rule = nullptr;
            elementStyle->getRule(ruleNames, &rule, canInherit);
            if (converter::BoolConverter::convert(rule, &value)) return value;
            return defaultBool;
        }

        int UiElement::computeSize(const std::vector<std::string> &ruleNames, int defaultSize, bool canInherit, int parentSize, bool *found) const {
            if (elementStyle == nullptr) return defaultSize;
            style::StyleValue *rule = nullptr;
            int size = 0;
            elementStyle->getRule(ruleNames, &rule, canInherit);
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
            if (elementStyle == nullptr) return defaultColor;
            style::StyleValue *rule = nullptr;
            SDL_Color color = SDL_Color();
            elementStyle->getRule(ruleNames, &rule, canInherit);
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
            UiElement *child = getChild();
            while (child != nullptr) {
                child->setWindow(window);
                child = child->getNext();
            }
        }

        void UiElement::setRenderer(SDL_Renderer *renderer) {
            this->renderer = renderer;
            UiElement *child = getChild();
            while (child != nullptr) {
                child->setRenderer(renderer);
                child = child->getNext();
            }
        }

        int UiElement::marginLeft(bool *found) const {
            if (!marginsActive) return 0;
            const UiElement *parent = getConstParent();
            return computeSize({"margin-left", "margin"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        }

        int UiElement::marginRight(bool *found) const {
            if (!marginsActive) return 0;
            const UiElement *parent = getConstParent();
            return computeSize({"margin-right", "margin"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        }

        int UiElement::marginTop(bool *found) const {
            if (!marginsActive) return 0;
            const UiElement *parent = getConstParent();
            return computeSize({"margin-top", "margin"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        }

        int UiElement::marginBottom(bool *found) const {
            if (!marginsActive) return 0;
            const UiElement *parent = getConstParent();
            return computeSize({"margin-bottom", "margin"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        }

        int UiElement::paddingLeft(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"padding-left", "padding"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        }

        int UiElement::paddingRight(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"padding-right", "padding"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        }

        int UiElement::paddingTop(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"padding-top", "padding"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        }

        int UiElement::paddingBottom(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"padding-bottom", "padding"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        }

        int UiElement::borderLeft(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"border-left", "border"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        }

        int UiElement::borderRight(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"border-right", "border"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        }

        int UiElement::borderTop(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"border-top", "border"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        }

        int UiElement::borderBottom(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"border-bottom", "border"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        }

        int UiElement::width(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"width"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), found);
        };

        int UiElement::height(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"height"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        };

        int UiElement::maxWidth(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"max-width"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        };

        int UiElement::minWidth(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"min-width"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        };

        int UiElement::maxHeight(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"max-height"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        };

        int UiElement::minHeight(bool *found) const {
            const UiElement *parent = getConstParent();
            return computeSize({"min-height"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), found);
        };

        SDL_Color UiElement::borderLeftColor() const { return computeColor({"border-left-color", "border-color"}); }

        SDL_Color UiElement::borderRightColor() const { return computeColor({"border-right-color", "border-color"}); }

        SDL_Color UiElement::borderTopColor() const { return computeColor({"border-top-color", "border-color"}); }

        SDL_Color UiElement::borderBottomColor() const { return computeColor({"border-bottom-color", "border-color"}); }

        SDL_Color UiElement::backgroundColor() const { return computeColor({"background-color"}, SDL_Color{255, 255, 255, 0}); }

        void UiElement::computeSelfLayout(int *width, int *height) const {
            (*width) = 50;
            (*height) = 50;
        }

        void UiElement::computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, std::list<std::tuple<int, int>> childsSizes) const {
            int childsWidth = 0;
            int childsHeight = 0;
            for (std::tuple<int, int> childSize : childsSizes) {
                childsWidth += std::get<0>(childSize);
                childsHeight += std::get<1>(childSize);
            }
            (*selfWidth) = childsWidth;
            (*selfHeight) = childsHeight;
        }

        void UiElement::tryRender(SDL_Rect oldClipRect) const {
            if (!styleManagerAvailable()) {
                renderChilds();
                return;
            }
            if (renderer == nullptr) throw NoRendererException();
            SDL_Rect clipRect;
            SDL_Rect finalClipRect;
            SDL_Rect clipRectNoBorders;
            SDL_Rect finalClipRectNoBorders;
            SDL_Rect clipRectNoPaddings;
            SDL_Rect finalClipRectNoPaddings;

            clipRect = SDL_Rect{oldClipRect.x, oldClipRect.y, this->elementRect.w, this->elementRect.h};
            renderScrollBars();

            finalClipRect = computeNewClipRect(&oldClipRect, &clipRect);

            clipRectNoBorders.x = clipRect.x + borderLeft();
            clipRectNoBorders.w = clipRect.w - borderLeft() - borderRight();
            clipRectNoBorders.y = clipRect.y + borderTop();
            clipRectNoBorders.h = clipRect.h - borderTop() - borderBottom();

            finalClipRectNoBorders = computeNewClipRect(&oldClipRect, &clipRectNoBorders);
            if (!SDL_SetRenderClipRect(renderer, &finalClipRectNoBorders)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't set clip rect no borders '%s'", SDL_GetError());
                return;
            }
            renderBackground();

            clipRectNoPaddings.x = clipRectNoBorders.x + paddingLeft();
            clipRectNoPaddings.w = clipRectNoBorders.w - paddingLeft() - paddingRight();
            clipRectNoPaddings.y = clipRectNoBorders.y + paddingTop();
            clipRectNoPaddings.h = clipRectNoBorders.h - paddingTop() - paddingBottom();
            finalClipRectNoPaddings = computeNewClipRect(&oldClipRect, &clipRectNoPaddings);

            if (!SDL_SetRenderClipRect(renderer, &finalClipRectNoPaddings)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't set clip rect no paddings '%s'", SDL_GetError());
                return;
            }

            renderSelfBeforeChilds();
            renderChilds();
            renderSelfAfterChilds();

            if (!SDL_SetRenderClipRect(renderer, &finalClipRect)) { // ensure rect has correct size
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't set clip rect '%s'", SDL_GetError());
                return;
            }
            renderBorders();
        }

        void UiElement::render() const {
            SDL_Rect clipRect;

            if (!SDL_GetRenderClipRect(renderer, &clipRect)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't get clip rect: '%s'", SDL_GetError());
                return;
            }

            tryRender(clipRect);

            if (!SDL_SetRenderClipRect(renderer, &clipRect)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't restore clip rect '%s'", SDL_GetError());
                return;
            }
        }

        void UiElement::renderChilds() const {
            SDL_Rect clipRect;
            SDL_Rect childClipRect;
            SDL_Rect childFinalClipRect;
            const UiElement *child = getConstChild();
            if (!SDL_GetRenderClipRect(renderer, &clipRect)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't get clip rect: '%s'", SDL_GetError());
                return;
            }
            while (child != nullptr) {
                childClipRect = SDL_Rect{std::min(std::max(clipRect.x, child->getXPos() + child->marginLeft()), clipRect.x + clipRect.w),
                                         std::min(std::max(clipRect.y, child->getYPos() + child->marginTop()), clipRect.y + clipRect.h),
                                         child->getWidth(), child->getHeight()};
                childClipRect.w = std::min(childClipRect.w + child->marginRight(), clipRect.w - (childClipRect.x - clipRect.x));
                childClipRect.h = std::min(childClipRect.h + child->marginBottom(), clipRect.h - (childClipRect.y - clipRect.y));
                childFinalClipRect = computeNewClipRect(&clipRect, &childClipRect);
                if (!SDL_SetRenderClipRect(renderer, &childFinalClipRect)) {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't set clip rect '%s'", SDL_GetError());
                    break;
                }
                child->render();
                child = child->getConstNext();
            }
            if (!SDL_SetRenderClipRect(renderer, &clipRect)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't restore clip rect '%s'", SDL_GetError());
                return;
            }
        }

        void UiElement::renderBorders() const {
            SDL_Color color;
            Uint8 r, g, b, a;
            SDL_FRect fRect;
            SDL_Rect clipRect;
            SDL_Rect realRect;
            SDL_Rect clippedRectWithBorders;
            int bLeft = borderLeft();
            int bRight = borderRight();
            int bTop = borderTop();
            int bBottom = borderBottom();

            if (!SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't get draw color '%s'", SDL_GetError());
                return;
            }
            getRect(&realRect);
            SDL_GetRenderClipRect(renderer, &clipRect);
            clippedRectWithBorders = computeNewClipRect(&clipRect, &realRect);
            // TODO: set border render order in the README

            // left border
            color = borderLeftColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(clippedRectWithBorders.x, clippedRectWithBorders.y + bTop, bLeft, clippedRectWithBorders.h - bTop - bBottom);
            SDL_RenderFillRect(renderer, &fRect);

            // right border
            color = borderRightColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(clippedRectWithBorders.x + clippedRectWithBorders.w, clippedRectWithBorders.y + bTop, -bRight,
                                clippedRectWithBorders.h - bTop - bBottom);
            SDL_RenderFillRect(renderer, &fRect);

            // top border
            color = borderTopColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(clippedRectWithBorders.x, clippedRectWithBorders.y, clippedRectWithBorders.w, bTop);
            SDL_RenderFillRect(renderer, &fRect);

            // bottom border
            color = borderBottomColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(clippedRectWithBorders.x, clippedRectWithBorders.y + clippedRectWithBorders.h, clippedRectWithBorders.w, -bBottom);
            SDL_RenderFillRect(renderer, &fRect);

            // restore previous color
            if (!SDL_SetRenderDrawColor(renderer, r, g, b, a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
        }

        void UiElement::renderBackground() const {
            Uint8 r, g, b, a;
            SDL_Rect rect;
            SDL_FRect fRect;

            if (!SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't get draw color '%s'", SDL_GetError());
                return;
            }

            SDL_Color color = backgroundColor();
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_GetRenderClipRect(renderer, &rect);

            SDL_RectToFRect(&rect, &fRect);
            SDL_RenderFillRect(renderer, &fRect);

            if (!SDL_SetRenderDrawColor(renderer, r, g, b, a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
        }

        void UiElement::renderScrollBar(int currentSize, int desiredSize) const {
            if (currentSize >= desiredSize && getNameStringFromRule("scroll-bar", {"always"}) == "") return;
        }

        void UiElement::renderScrollBars() const {
            // renderScrollBar();
            // renderScrollBar();
        }

        void UiElement::focus(bool focused) {
            _focus = focused;
            if (focused) onFocusGet();
            else onFocusLoose();
        }
    } // namespace element
} // namespace gui
