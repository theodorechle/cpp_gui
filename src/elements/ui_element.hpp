#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "../converters/bool_converter.hpp"
#include "../converters/color_converter.hpp"
#include "../converters/number_converter.hpp"
#include "../converters/size_converter.hpp"
#include "abstract_element.hpp"
#include "managers/manager_actions_service.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <iostream>
#include <vector>

namespace gui {
    namespace element {

        class UIElement : public AbstractElement {
            SDL_Rect elementRect = SDL_Rect{0, 0, 0, 0};
            int elementDesiredWidth = 0;
            int elementDesiredHeight = 0;
            bool sizeParentRelative = false;
            struct {
                int x;
                int y;
            } currentScrollPos = {0, 0};
            SDL_Window *window;
            SDL_Renderer *renderer = nullptr;
            TTF_TextEngine *textEngine = nullptr;
            bool marginsActive = true;
            bool _focus = false;
            ManagerActionsService *managerActionsService = nullptr;

            /**
             * compute desired layout without margins, paddings, borders, ...
             */
            virtual void computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight);

            void setRect(const SDL_Rect &rect);
            void setPos(int x, int y);
            void setSize(int width, int height);
            void setDesiredSize(int width, int height);

            void setParent(UIElement *parent) { AbstractElement::setParent(parent); }

            /**
             * Calls the rendering methods in this order:
             * - renderBackground
             * - renderSelfBeforeChilds
             * - renderChilds
             * - renderSelfAfterChilds
             * - renderBorders
             * Also manages the clip rect for borders and content
             */
            void tryRender(SDL_Rect oldClipRect);

            /**
             * Return a modified version of wantedNewClipRect who fits in oldClipRect
             */
            static SDL_Rect computeNewClipRect(SDL_Rect *oldClipRect, SDL_Rect *wantedNewClipRect);

            virtual void onFocusGet() {}
            virtual void onFocusLoose() {}

            void renderChilds() override;
            void renderBackground() const;
            void renderScrollBar(int currentSize, int desiredSize) const;
            void renderScrollBars() const;
            void renderBorders();

        protected:
            TTF_TextEngine *getTextEngine() { return textEngine; }
            static SDL_FRect createFRect(int x, int y, int width, int height);

            int getIntFromRule(const std::vector<std::string> &styleNames, int defaultSize = 0, bool canInherit = false) const;
            std::string getStringFromRule(const std::vector<std::string> &styleNames, const std::string &defaultString = "",
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
             * If no allowed value is given, it will return the found value.
             */
            std::string getNameStringFromRule(const std::vector<std::string> &styleNames, const std::vector<std::string> &allowedValues,
                                              const std::string &defaultString = "", bool canInherit = false) const;

            bool getBoolFromRule(const std::vector<std::string> &styleNames, bool defaultBool = false, bool canInherit = false) const;

            /**
             * If any of the style names is found in current loaded style, returns the corresponding value.
             * Else returns default;
             */
            int computeSize(const std::vector<std::string> &styleNames, int defaultSize = 0, bool canInherit = false, int parentSize = 0,
                            bool *found = nullptr);

            /**
             * If any of the style names is found in current loaded style, returns the corresponding value.
             * Else returns default;
             */
            SDL_Color computeColor(const std::vector<std::string> &styleNames, SDL_Color defaultColor = SDL_Color{0, 0, 0, 255},
                                   bool canInherit = false) const;

            void askRendering() const;
            void askRecomputeLayout() const;

        public:
            UIElement(std::string elementName, gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr,
                      std::vector<std::string> *classes = nullptr, const std::string &identifier = "", TTF_TextEngine *textEngine = nullptr)
                : AbstractElement{elementName, elementsStyleManager, classes, identifier}, textEngine{textEngine} {}

            UIElement *getParent() { return static_cast<UIElement *>(AbstractElement::getParent()); }
            const UIElement *getConstParent() const { return static_cast<const UIElement *>(AbstractElement::getConstParent()); }
            void addChild(UIElement *child);
            UIElement *getChild() { return static_cast<UIElement *>(AbstractElement::getChild()); }
            void setNext(UIElement *next) { AbstractElement::setNext(next); }
            UIElement *getNext() { return static_cast<UIElement *>(AbstractElement::getNext()); }

            void setWindow(SDL_Window *window) { this->window = window; }
            void setRenderer(SDL_Renderer *renderer) { this->renderer = renderer; }
            SDL_Renderer *getRenderer() const { return renderer; }
            SDL_Window *getWindow() const { return window; }

            void setMarginsActive(bool active) { marginsActive = active; }

            int getWidth() const { return elementRect.w; };
            int getHeight() const { return elementRect.h; };
            int getXPos() const { return elementRect.x; };
            int getYPos() const { return elementRect.y; };

            void getRect(SDL_Rect *rect) const;
            void getPos(int *x, int *y) const;
            void getSize(int *width, int *height) const;
            void getDesiredSize(int *width, int *height) const;

            int marginLeft(bool *found = nullptr);
            int marginRight(bool *found = nullptr);
            int marginTop(bool *found = nullptr);
            int marginBottom(bool *found = nullptr);

            int paddingLeft(bool *found = nullptr);
            int paddingRight(bool *found = nullptr);
            int paddingTop(bool *found = nullptr);
            int paddingBottom(bool *found = nullptr);

            int borderLeft(bool *found = nullptr);
            int borderRight(bool *found = nullptr);
            int borderTop(bool *found = nullptr);
            int borderBottom(bool *found = nullptr);

            int width(bool *found = nullptr);
            int height(bool *found = nullptr);
            int maxWidth(bool *found = nullptr);
            int minWidth(bool *found = nullptr);
            int maxHeight(bool *found = nullptr);
            int minHeight(bool *found = nullptr);

            SDL_Color borderLeftColor() const;
            SDL_Color borderRightColor() const;
            SDL_Color borderTopColor() const;
            SDL_Color borderBottomColor() const;

            SDL_Color backgroundColor() const;

            bool isSizeParentRelative() const { return sizeParentRelative; }
            bool areAllParentSizesParentRelative() const;

            void computeLayout(int x, int y, int availableWidth, int availableHeight) override final;
            void computeChildsLayout(int x, int y, int availableWidth, int availableHeight) override;

            void computeDesiredLayout(int *desiredWidth, int *desiredHeight) final;

            virtual void catchEvent(const SDL_Event &event) {}

            /**
             * Updates the renderer clip rect and call tryRender
             */
            void render() override final;

            void focus(bool focused);
            bool focus() { return _focus; }

            void setManagerActionsService(gui::element::ManagerActionsService *managerActionsService);
        };

        class NoRendererException : public std::logic_error {
        public:
            NoRendererException() : std::logic_error{"No renderer available"} {};
        };

    } // namespace element
} // namespace gui

#endif // UI_ELEMENT_HPP
