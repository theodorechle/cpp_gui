#include "ui_manager.hpp"

namespace gui {
    namespace element {
        namespace manager {
            UIManager::UIManager(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect *clipRect) : window{window}, renderer{renderer} {
                if (clipRect != nullptr) {
                    this->clipRect = *clipRect;
                    canChangeSize = false;
                }
                else {
                    updateClipRect();
                }
            }

            void UIManager::setElementsTree(gui::element::AbstractElement *element) {
                AbstractManager::setElementsTree(new gui::element::RootElement());
                static_cast<gui::element::UIElement *>(elementsTree)->setRenderer(renderer);
                static_cast<gui::element::UIElement *>(elementsTree)->setWindow(window);
                static_cast<gui::element::UIElement *>(elementsTree)->setManagerActionsService(getManagerActionsService());
                computeFinalElementsLayout();
                static_cast<gui::element::UIElement *>(element)->setRenderer(renderer);
                static_cast<gui::element::UIElement *>(element)->setWindow(window);
                elementsTree->addChild(static_cast<gui::element::UIElement *>(element));
            }

            void UIManager::updateClipRect() {
                if (!canChangeSize) return;
                int width = 0;
                int height = 0;
                if (!SDL_GetCurrentRenderOutputSize(renderer, &width, &height)) {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't get render size");
                }
                this->clipRect = SDL_Rect{0, 0, width, height};
            }

            void UIManager::computeDesiredElementsLayout(int *width, int *height) {
                if (elementsTree == nullptr) return;
                (*width) = 0;
                (*height) = 0;
                elementsTree->computeDesiredLayout(width, height);
            }

            void UIManager::computeFinalElementsLayout() {
                if (elementsTree == nullptr) return;
                elementsTree->computeLayout(clipRect.x, clipRect.y, clipRect.w, clipRect.h);
            }

            void UIManager::computeElementsLayout() {
                int width, height;
                // TODO: find a cleaner way to keep the root element to the window size
                updateClipRect();
                computeFinalElementsLayout();
                computeDesiredElementsLayout(&width, &height);
                computeFinalElementsLayout();
                computeDesiredElementsLayout(&width, &height);
                computeFinalElementsLayout();
            }

            void UIManager::renderElements(bool clear) const {
                if (elementsTree == nullptr) return;
                Uint8 r, g, b, a;
                int width = 0, height = 0;
                if (clear && SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderClear(renderer);
                    SDL_SetRenderDrawColor(renderer, r, g, b, a);
                }
                SDL_GetCurrentRenderOutputSize(renderer, &width, &height);
                SDL_SetRenderClipRect(renderer, &clipRect);
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
                    processMouseEvents();
                    break;
                case SDL_EVENT_WINDOW_RESIZED:
                    askRecomputeLayout();
                    break;
                default:
                    break;
                }

                sendEvent(event, focusedElement);
            }

            void UIManager::processMouseEvents() {
                if (elementsTree == nullptr) return;
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
                needRecomputeLayout(true);
                // TODO: don't recalculate the entire tree
                // TODO: recalculate and re-render only when an element has changed
            }

        } // namespace manager
    } // namespace element
} // namespace gui
