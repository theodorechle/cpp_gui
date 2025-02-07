#include "ui_manager.hpp"

namespace gui {
    namespace element {
        namespace manager {

            void UIManager::setElementsTree(gui::element::AbstractElement *element) {
                AbstractManager::setElementsTree(new gui::element::RootElement());
                static_cast<gui::element::UIElement *>(elementsTree)->setRenderer(renderer);
                static_cast<gui::element::UIElement *>(elementsTree)->setWindow(window);
                static_cast<gui::element::UIElement *>(element)->setRenderer(renderer);
                static_cast<gui::element::UIElement *>(element)->setWindow(window);
                elementsTree->addChild(static_cast<gui::element::UIElement *>(element));
                static_cast<gui::element::UIElement *>(elementsTree)->setManagerActionsService(getRedrawRequester());
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
                    break;
                case SDL_EVENT_WINDOW_RESIZED:
                    computeElementsLayout();
                    break;
                case SDL_EVENT_TEXT_INPUT:
                    // case SDL_EVENT_KEY_DOWN:
                    // guiEvent = gui::Event::KeyDown;
                    // std::cerr << SDL_GetKeyName(event.key.key) << "\n";
                    // std::cerr << event.text.text << "\n";
                    break;
                default:
                    break;
                }

                sendEvent(event, focusedElement);
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
                        if (focusedElement != nullptr) focusedElement->focus(false);
                        focusedElement = clickedElement;
                        if (focusedElement != nullptr) focusedElement->focus(true);
                        setElementsModifierState("clicked", clickedElement, true, SDL_Event{SDL_EVENT_MOUSE_BUTTON_DOWN});
                    }
                }
                else {
                    if (clickedElement != nullptr) {
                        setElementsModifierState("clicked", clickedElement, false, SDL_Event{SDL_EVENT_MOUSE_BUTTON_DOWN});
                        clickedElement = nullptr;
                    }
                }
                if (hoveredElement != currentHoveredElement) {
                    setElementsModifierState("hovered", hoveredElement, false, SDL_Event{SDL_EVENT_MOUSE_MOTION});
                    setElementsModifierState("hovered", currentHoveredElement, true, SDL_Event{SDL_EVENT_MOUSE_MOTION});
                    hoveredElement = currentHoveredElement;
                }
            }

            void UIManager::sendEvent(const SDL_Event &event, UIElement *leafElement) {
                UIElement *element = leafElement;
                if (element == nullptr) return;
                while (element != nullptr) {
                    element->catchEvent(event);
                    element = element->getParent();
                }
            }

            void UIManager::setElementsModifierState(const std::string &modifier, UIElement *leafElement, bool enabled, const SDL_Event &event) {
                UIElement *element = leafElement;
                if (element == nullptr) return;
                while (element != nullptr) {
                    element->setModifierState(modifier, enabled);
                    if (enabled) element->catchEvent(event);
                    element = element->getParent();
                }
                computeElementsLayout(); // TODO: don't recalculate the entire tree
                needRendering(true);     // TODO: recalculate and re-render only when an element has changed
            }

        } // namespace manager
    } // namespace element
} // namespace gui
