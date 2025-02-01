#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "../converters/color_converter.hpp"
#include "../converters/size_converter.hpp"
#include "abstract_element.hpp"

#include <SDL3/SDL.h>
#include <algorithm>
#include <iostream>
#include <vector>

namespace gui {
    namespace element {

        class UIElement : public AbstractElement {
            SDL_Rect elementRect = SDL_Rect{0, 0, 0, 0};
            int elementDesiredWidth = 0;
            int elementDesiredHeight = 0;
            SDL_Renderer *renderer = nullptr;

            /**
             * compute desired layout without margins, paddings, borders, ...
             */
            virtual void computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight);

            /**
             * If any of the style names is found in current loaded style, returns the corresponding value.
             * Else returns default;
             */
            int computeSize(const std::vector<std::string> &styleNames, int defaultSize = 0, bool canInherit = false, int parentSize = 0) const;

            /**
             * If any of the style names is found in current loaded style, returns the corresponding value.
             * Else returns default;
             */
            SDL_Color computeColor(const std::vector<std::string> &styleNames, SDL_Color defaultColor = SDL_Color{0, 0, 0, 255},
                                   bool canInherit = false) const;

            void setRect(SDL_Rect rect);
            void setPos(int x, int y);
            void setSize(int width, int height);
            void setDesiredSize(int width, int height);

            static SDL_FRect createFRect(int x, int y, int width, int height);
            void setParent(UIElement *parent) { AbstractElement::setParent(parent); }

            void tryRender(SDL_Rect oldClipRect);

        public:
            UIElement(std::string elementName, gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr,
                      std::vector<std::string> *classes = nullptr, const std::string &identifier = "")
                : AbstractElement{elementName, elementsStyleManager, classes, identifier} {}

            UIElement *getParent() { return static_cast<UIElement *>(AbstractElement::getParent()); }
            const UIElement *getConstParent() const { return static_cast<const UIElement *>(AbstractElement::getConstParent()); }
            void addChild(UIElement *child);
            UIElement *getChild() { return static_cast<UIElement *>(AbstractElement::getChild()); }
            void setNext(UIElement *next) { AbstractElement::setNext(next); }
            UIElement *getNext() { return static_cast<UIElement *>(AbstractElement::getNext()); }

            void setRenderer(SDL_Renderer *renderer) { this->renderer = renderer; }
            SDL_Renderer *getRenderer() const { return renderer; }

            int getWidth() const { return elementRect.w; };
            int getHeight() const { return elementRect.h; };
            int getXPos() const { return elementRect.x; };
            int getYPos() const { return elementRect.y; };

            void getRect(SDL_Rect *rect) const;
            void getPos(int *x, int *y) const;
            void getSize(int *width, int *height) const;
            void getDesiredSize(int *width, int *height) const;

            int marginLeft() const;
            int marginRight() const;
            int marginTop() const;
            int marginBottom() const;

            int paddingLeft() const;
            int paddingRight() const;
            int paddingTop() const;
            int paddingBottom() const;

            int borderLeft() const;
            int borderRight() const;
            int borderTop() const;
            int borderBottom() const;

            SDL_Color borderLeftColor() const;
            SDL_Color borderRightColor() const;
            SDL_Color borderTopColor() const;
            SDL_Color borderBottomColor() const;

            SDL_Color backgroundColor() const;

            void computeLayout(int x, int y, int availableWidth, int availableHeight) override;
            void computeDesiredLayout(int *desiredWidth, int *desiredHeight) final;

            /**
             * Computes the layout of the element and draw it.
             * Draw the border and call renderSelf to render the element content.
             * To override the rendering, see te renderSelf method.
             */
            void render() override final;
            void renderChilds() override;
            void renderBackground() const;
            void renderBorder() const;
        };

        class NoRendererException : public std::logic_error {
        public:
            NoRendererException() : std::logic_error{"No renderer available"} {};
        };

    } // namespace element
} // namespace gui

#endif // UI_ELEMENT_HPP
