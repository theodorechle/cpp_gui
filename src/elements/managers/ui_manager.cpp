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
                int width = 0, height = 0;
                elementsTree->computeDesiredLayout(&width, &height);
                SDL_GetCurrentRenderOutputSize(renderer, &width, &height);
                elementsTree->computeLayout(0, 0, width, height);
            }

            void UIManager::renderElements() const {
                Uint8 r, g, b, a;
                if (SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderClear(renderer);
                    SDL_SetRenderDrawColor(renderer, r, g, b, a);
                }
                elementsTree->render();
                SDL_RenderPresent(renderer);
            }

            void UIManager::processEvent(const SDL_Event &event) {
                if (event.type == SDL_EVENT_QUIT) {
                    status(Status::ENDED);
                    return;
                }
            }

            void UIManager::processMouseEvents() {
                float x, y;
                SDL_MouseButtonFlags mouseFlags = SDL_GetMouseState(&x, &y);
                SDL_Point mousePos = SDL_Point{(int)x, (int)y};
                UIElement *currentElement = static_cast<UIElement *>(elementsTree);
                SDL_Rect currentElementRect;
                while (currentElement != nullptr) {
                    currentElement->getRect(&currentElementRect);
                    if (SDL_PointInRect(&mousePos, &currentElementRect)) {
                        if (currentElement->getChild() == nullptr) {
                            break;
                        }
                        currentElement = currentElement->getChild();
                    }
                    else currentElement = currentElement->getNext();
                }

                if (mouseFlags) {
                    if (clickedElement == nullptr) {
                        clickedElement = currentElement;
                        setElementsModifierState("clicked", clickedElement, true, gui::Event::Clicked);
                    }
                }
                else {
                    if (clickedElement != nullptr) {
                        setElementsModifierState("clicked", clickedElement, false, gui::Event::Hovered);
                        clickedElement = nullptr;
                    }
                    if (hoveredElement != currentElement) {
                        setElementsModifierState("hovered", hoveredElement, false, gui::Event::Clicked); // FIXME: doesn't work
                        setElementsModifierState("hovered", currentElement, true, gui::Event::Clicked);
                        hoveredElement = currentElement;
                    }
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
            }

        } // namespace manager
    } // namespace element
} // namespace gui
