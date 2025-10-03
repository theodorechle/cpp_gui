#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "../../converters/bool_converter.hpp"
#include "../../converters/color_converter.hpp"
#include "../../converters/number_converter.hpp"
#include "../../converters/size_converter.hpp"
#include "../abstracts/abstract_element.hpp"
#include "utils.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <iostream>
#include <vector>

namespace gui {
    namespace element {

        class UiElement : public AbstractElement {
            SDL_Rect elementRect = SDL_Rect{0, 0, 0, 0}; // TODO: update it to be able to use percentages
            SDL_Window *window;
            SDL_Renderer *renderer = nullptr;
            TTF_TextEngine *textEngine = nullptr;
            bool _focus = false;

            void setParent(UiElement *parent) { AbstractElement::parent(parent); }

            /**
             * Return a modified version of wantedNewClipRect who fits in oldClipRect
             * Does not alter any of the given rects
             */
            static SDL_Rect computeNewClipRect(SDL_Rect *oldClipRect, SDL_Rect *wantedNewClipRect);

            virtual void onFocusGet() {}
            virtual void onFocusLoose() {}

        protected:
            TTF_TextEngine *getTextEngine() const { return textEngine; }
            static SDL_FRect createFRect(int x, int y, int width, int height);

            int getIntFromRule(const std::vector<std::string> &ruleNames, int defaultSize = 0, bool canInherit = false) const;
            std::string getStringFromRule(const std::vector<std::string> &ruleNames, const std::string &defaultString = "",
                                          bool canInherit = false) const;

            /**
             * Name strings are values who are valid rule names, but since they are values, they are considered as strings without quotes.
             * For example, in
             ```
             list {
                childs-size: biggest;
             }
             ```
             * 'biggest' would be a valid rule name, as 'childs-size' is, but is considered as a string because it's a value.
             *
             * Specifically, name strings should be a set of possible values, restraining the possibilities, whereas strings could be anything,
             although you can still put anything in name strings.
             * It's just not what they were made for in the first place.
             *
             * If no allowed value is given, it will return the found value.
             * If no value is found, it returns the defaultValue.
             */
            std::string getNameStringFromRule(const std::string &ruleName, const std::vector<std::string> &allowedValues,
                                              const std::string &defaultValue = "", bool canInherit = false) const;

            /**
             * Name strings are values who are valid rule names, but since they are values, they are considered as strings without quotes.
             * For example, in
             ```
             list {
                childs-size: biggest;
             }
             ```
             * 'biggest' would be a valid rule name, as 'childs-size' is, but is considered as a string because it's a value.
             *
             * Specifically, name strings should be a set of possible values, restraining the possibilities, whereas strings could be anything,
             although you can still put anything in name strings.
             * It's just not what they were made for in the first place.
             *
             * If no allowed value is given, it will return the found value.
             * If no value is found, it returns the defaultValue.
             */
            std::string getNameStringFromRules(const std::vector<std::string> &ruleNames, const std::vector<std::string> &allowedValues,
                                               const std::string &defaultValue = "", bool canInherit = false) const;
            bool getBoolFromRule(const std::vector<std::string> &ruleNames, bool defaultBool = false, bool canInherit = false) const;

            /**
             * If any of the style names is found in current loaded style, returns the corresponding value.
             * Else returns default;
             */
            int computeSize(const std::vector<std::string> &ruleNames, int defaultSize = 0, bool canInherit = false, int parentSize = 0,
                            bool *found = nullptr) const;

            /**
             * If any of the style names is found in current loaded style, returns the corresponding value.
             * Else returns default;
             */
            SDL_Color computeColor(const std::vector<std::string> &ruleNames, SDL_Color defaultColor = SDL_Color{0, 0, 0, 255},
                                   bool canInherit = false) const;

        public:
            UiElement(std::string elementName, gui::elementStyle::manager::StyleNodesManager *elementsStyleManager = nullptr,
                      std::vector<std::string> *classes = nullptr, const std::string &identifier = "", TTF_TextEngine *textEngine = nullptr)
                : AbstractElement{elementName, elementsStyleManager, classes, identifier}, textEngine{textEngine} {}

            UiElement *parent() { return static_cast<UiElement *>(AbstractElement::parent()); }
            const UiElement *constParent() const { return static_cast<const UiElement *>(AbstractElement::constParent()); }
            void addChild(UiElement *child);
            UiElement *child() { return static_cast<UiElement *>(AbstractElement::child()); }
            const UiElement *constChild() const { return static_cast<const UiElement *>(AbstractElement::constChild()); }
            void next(UiElement *next) { AbstractElement::next(next); }
            UiElement *next() { return static_cast<UiElement *>(AbstractElement::next()); }
            const UiElement *constNext() const { return static_cast<const UiElement *>(AbstractElement::constNext()); }

            void setWindow(SDL_Window *window);
            void setRenderer(SDL_Renderer *renderer);
            SDL_Renderer *getRenderer() const { return renderer; }
            SDL_Window *getWindow() const { return window; }

            int getWidth() const { return elementRect.w; };
            int getHeight() const { return elementRect.h; };
            int getXPos() const { return elementRect.x; };
            int getYPos() const { return elementRect.y; };

            int marginLeft(bool *found = nullptr) const;
            int marginRight(bool *found = nullptr) const;
            int marginTop(bool *found = nullptr) const;
            int marginBottom(bool *found = nullptr) const;

            int paddingLeft(bool *found = nullptr) const;
            int paddingRight(bool *found = nullptr) const;
            int paddingTop(bool *found = nullptr) const;
            int paddingBottom(bool *found = nullptr) const;

            int borderLeft(bool *found = nullptr) const;
            int borderRight(bool *found = nullptr) const;
            int borderTop(bool *found = nullptr) const;
            int borderBottom(bool *found = nullptr) const;

            int width(bool *found = nullptr) const;
            int height(bool *found = nullptr) const;
            int maxWidth(bool *found = nullptr) const;
            int minWidth(bool *found = nullptr) const;
            int maxHeight(bool *found = nullptr) const;
            int minHeight(bool *found = nullptr) const;

            SDL_Color borderLeftColor() const;
            SDL_Color borderRightColor() const;
            SDL_Color borderTopColor() const;
            SDL_Color borderBottomColor() const;

            SDL_Color backgroundColor() const;

            /**
             * Must be tree independant (no use of parent or childs nor nexts)
             */
            virtual void initBeforeLayoutComputing() {}

            /**
             * Must be tree independant (no use of parent or childs nor nexts)
             */
            virtual void restoreAfterLayoutComputing() {}

            /**
             * Adds the borders, margins, paddings to the element layout (width and height).
             */
            virtual void computeTotalLayout(int *width, int *height) const;

            /**
             * selfWidth and selfHeight are the new sizes
             * selfWidthWithoutChilds and selfHeightWithoutChilds are the previous sizes computed by computeSelfLayout
             */
            virtual void computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                                    std::list<std::tuple<int, int>> childsSizes) const;

            /**
             * Should compute only the layout of the content of the element, not borders, margins, paddings.
             * To change the behavior of these, see the computeSelfLayout function
             */
            virtual void computeInnerLayout(int *width, int *height) const;

        private:
            bool setClipRect(const SDL_Rect *clipRect, std::string callerName = "") const;

            virtual void renderSelfBeforeChilds() const;
            virtual void renderSelfAfterChilds() const;
            virtual void renderChilds(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                      std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const;
            virtual void renderBackground() const;
            virtual void renderBorders() const;
            virtual void renderVerticalScrollBar(int totalHeight, ui::Size clippedSize, int offset) const;
            virtual void renderHorizontalScrollBar(int totalWidth, ui::Size clippedSize, int offset) const;

        public:
            void renderSelfBeforeChildsWrapper() const override;
            void renderSelfAfterChildsWrapper() const override;
            void renderChildsWrapper(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                     std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const override;
            bool renderSingleChildWrapper(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                          std::function<const ElementData *(const AbstractElement *)> childInfosCallback, const UiElement *child,
                                          gui::element::ui::Pos pos) const;
            void renderBackgroundWrapper() const;
            void renderBordersWrapper() const;
            void renderVerticalScrollBarWrapper(int totalHeight, ui::Size clippedSize, int offset) const;
            void renderHorizontalScrollBarWrapper(int totalWidth, ui::Size clippedSize, int offset) const;

            /**
             * Calls the rendering methods in this order:
             * - renderBordersWrapper
             * - renderBackgroundWrapper
             * - renderSelfBeforeChildsWrapper
             * - renderChildsWrapper
             * - renderScrollBarsWrapper
             * Also manages the clip rect for content (everything except borders)
             */
            bool render(const ElementData *elementData, std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                        std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const override;

            virtual void catchEvent(const SDL_Event &event) {}

            void focus(bool focused);
            bool focus() { return _focus; };
        };

        class NoRendererException : public std::runtime_error {
        public:
            NoRendererException() : std::runtime_error{"No renderer available"} {};
        };

    } // namespace element
} // namespace gui

#endif // UI_ELEMENT_HPP
