#include "ui_manager.hpp"

namespace gui {
    namespace element {
        namespace manager {

            void UIManager::setElementsTree(gui::element::AbstractElement *element) {
                AbstractManager::setElementsTree(new gui::element::RootElement());
                static_cast<gui::element::UIElement *>(elementsTree)->setRenderer(static_cast<gui::element::UIElement *>(element)->getRenderer());
                elementsTree->addChild(static_cast<gui::element::UIElement *>(element));
            }

            void UIManager::computeElementsLayout() {
                if (elementsTree == nullptr) return;
                int width = 0, height = 0;
                elementsTree->computeDesiredLayout(&width, &height);
                SDL_GetCurrentRenderOutputSize(renderer, &width, &height);
                elementsTree->computeLayout(0, 0, width, height);
            }

            void UIManager::renderElements() const {
                Uint8 r, g, b, a;
                int width = 0, height = 0;
                SDL_Rect rect;
                if (SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderClear(renderer);
                    SDL_SetRenderDrawColor(renderer, r, g, b, a);
                }
                SDL_GetCurrentRenderOutputSize(renderer, &width, &height);
                rect = SDL_Rect{0, 0, width, height};
                SDL_SetRenderClipRect(renderer, &rect);
                elementsTree->render();
                SDL_RenderPresent(renderer);
            }

            void UIManager::processEvent(const SDL_Event &event) {
                switch (event.type) {
                case SDL_EVENT_QUIT:
                    status(Status::ENDED);
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    mouseEventsOccurred = true;
                default:
                    break;
                }
            }

            void UIManager::processMouseEvents() {
                if (!mouseEventsOccurred) return;
                mouseEventsOccurred = false;
                float x, y;
                SDL_MouseButtonFlags mouseFlags = SDL_GetMouseState(&x, &y);
                SDL_Point mousePos = SDL_Point{(int)x, (int)y};
                UIElement *currentElement = static_cast<UIElement *>(elementsTree->getChild());
                UIElement *currentHoveredElement = nullptr;
                SDL_Rect currentElementRect;
                while (currentElement != nullptr) {
                    currentElement->getRect(&currentElementRect);
                    currentElementRect.x += currentElement->marginLeft();
                    currentElementRect.y += currentElement->marginTop();
                    if (SDL_PointInRect(&mousePos, &currentElementRect)) {
                        currentHoveredElement = currentElement;
                        if (currentElement->getChild() == nullptr) {
                            break;
                        }
                        currentElement = currentElement->getChild();
                    }
                    else currentElement = currentElement->getNext();
                }

                if (mouseFlags) {
                    if (clickedElement == nullptr) {
                        clickedElement = currentHoveredElement;
                        setElementsModifierState("clicked", clickedElement, true, gui::Event::Clicked);
                    }
                }
                else {
                    if (clickedElement != nullptr) {
                        setElementsModifierState("clicked", clickedElement, false, gui::Event::Hovered);
                        clickedElement = nullptr;
                    }
                }
                if (hoveredElement != currentHoveredElement) {
                    setElementsModifierState("hovered", hoveredElement, false, gui::Event::Clicked);
                    setElementsModifierState("hovered", currentHoveredElement, true, gui::Event::Clicked);
                    hoveredElement = currentHoveredElement;
                }
            }

            void UIManager::setElementsModifierState(const std::string &modifier, UIElement *leafElement, bool enabled, gui::Event event) {
                UIElement *element = leafElement;
                if (element == nullptr) return;
                needRendering(true);
                while (element != nullptr) {
                    element->setModifierState(modifier, enabled);
                    if (enabled) element->catchEvent(event);
                    element = element->getParent();
                }
                computeElementsLayout();
            }

        } // namespace manager
    } // namespace element
} // namespace gui
