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
                    updateRenderingData();
                }
            }

            void UIManager::createRootElement() {
                gui::element::UiElement *rootElement = new gui::element::RootElement();
                rootElement->setRenderer(renderer);
                rootElement->setWindow(window);
                elementsTree = rootElement;
                rootRenderNode = new gui::element::ui::render::UiRenderNode(renderer, nullptr, rootElement);
            }

            void UIManager::updateRenderingData() {
                if (!canChangeSize) return;
                int width = 0;
                int height = 0;
                if (!SDL_GetCurrentRenderOutputSize(renderer, &width, &height)) {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UIManager::updateRenderingData: Can't get render size");
                }
                this->clipRect = SDL_Rect{0, 0, width, height};
                renderedTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, clipRect.w, clipRect.h);
                if (renderedTexture == nullptr) {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UIManager::updateRenderingData: Can't create a texture for rendering: %s", SDL_GetError());
                }
            }

            void UIManager::addChildToRootElement(gui::element::AbstractElement *childElement) {
                static_cast<gui::element::UiElement *>(elementsTree)->addChild(static_cast<gui::element::UiElement *>(childElement));
            }

            void UIManager::resetEvents() {
                clickedElement = nullptr;
                hoveredElement = nullptr;
                focusedElement = nullptr;
                needUpdate();
            }

            void UIManager::computeNodeLayout(gui::element::ui::render::UiRenderNode *node) { node->computeSelfLayout(); }

            void UIManager::initElementsBeforeLayoutComputing(gui::element::ui::render::UiRenderNode *rootRenderNode) {
                rootRenderNode->initBeforeLayoutComputing();
            }

            void UIManager::restoreAfterLayoutComputing(gui::element::ui::render::UiRenderNode *rootRenderNode) {
                rootRenderNode->restoreAfterLayoutComputing();
            }

            void UIManager::prepareRenderNodes(UiElement *rootElement, gui::element::ui::render::UiRenderNode *rootRenderNode, bool isRoot) {
                UiElement *currentElement = rootElement;
                gui::element::ui::render::UiRenderNode *currentRenderNode;
                while (currentElement != nullptr) {
                    if (isRoot) {
                        currentRenderNode = rootRenderNode;
                    }
                    else {
                        currentRenderNode = gui::element::ui::render::elementToRenderNodeConverter(renderer, rootRenderNode, currentElement);
                        rootRenderNode->addChild(currentRenderNode);
                    }
                    computeNodeLayout(currentRenderNode);
                    prepareRenderNodes(currentElement->getChild(), currentRenderNode);
                    currentElement = currentElement->getNext();
                }
            }

            void UIManager::computeNodesAndChildsLayout(gui::element::ui::render::UiRenderNode *node) { node->computeSelfAndChildsLayout(); }

            void UIManager::computeNodesRelativeLayout(gui::element::ui::render::UiRenderNode *node) {
                gui::element::ui::render::UiRenderNode *currentNode = node;
                while (currentNode != nullptr) {
                    currentNode->computeRelativeLayout();
                    currentNode = currentNode->next();
                }
            }

            void UIManager::computeNodesFinalLayout(gui::element::ui::render::UiRenderNode *node, SDL_Rect *rootClipRect) {
                if (node == nullptr) return;
                if (rootClipRect == nullptr) node->computeFinalLayout();
                else node->computeFinalLayout(*rootClipRect);
            }

            void UIManager::computeElementsLayout() {
                if (rootRenderNode == nullptr) return;
                initElementsBeforeLayoutComputing(rootRenderNode);
                prepareRenderNodes(static_cast<UiElement *>(elementsTree), rootRenderNode, true);
                computeNodesAndChildsLayout(rootRenderNode);
                computeNodesRelativeLayout(rootRenderNode);
                computeNodesFinalLayout(rootRenderNode, &clipRect);
                restoreAfterLayoutComputing(rootRenderNode);
            }

            void UIManager::createRenderedTexture() {
                if (!needRenderingUpdate) return;
                needRenderingUpdate = false;
                if (rootRenderNode == nullptr) return;
                SDL_SetRenderTarget(renderer, renderedTexture);
                rootRenderNode->render();
                SDL_SetRenderTarget(renderer, nullptr);
            }

            void UIManager::renderElements(bool clear) const {
                std::cerr << "Rendering elements (renderedTexture is null: " << (renderedTexture == nullptr) << ")\n";
                if (renderedTexture != nullptr) {
                    std::cerr << "renderedTexture: w=" << renderedTexture->w << ", h=" << renderedTexture->h << "\n";
                    SDL_RenderTexture(renderer, renderedTexture, nullptr, nullptr);
                }
                SDL_RenderPresent(renderer);
                // if (elementsTree == nullptr) return;
                // Uint8 r, g, b, a;
                // int width = 0, height = 0;
                // if (clear && SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
                //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                //     SDL_RenderClear(renderer);
                //     SDL_SetRenderDrawColor(renderer, r, g, b, a);
                // }
                // SDL_GetCurrentRenderOutputSize(renderer, &width, &height);
                // SDL_SetRenderClipRect(renderer, &clipRect);
                // elementsTree->render();
                // SDL_RenderPresent(renderer);
            }

            void UIManager::update() {
                computeElementsLayout();
                createRenderedTexture();
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
                    needUpdate(true);
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
                UiElement *currentElement = static_cast<UiElement *>(elementsTree->child());
                UiElement *currentHoveredElement = nullptr;
                SDL_Rect currentElementRect;
                while (currentElement != nullptr) {
                    // currentElement->getRect(&currentElementRect); // FIXME
                    currentElementRect.x += currentElement->marginLeft();
                    currentElementRect.y += currentElement->marginTop();
                    if (SDL_PointInRect(&mousePos, &currentElementRect)) {
                        currentHoveredElement = currentElement;
                        currentElement = currentElement->getChild();
                    }
                    else {
                        currentElement = currentElement->getNext();
                    }
                }
                if (mouseFlags) {
                    if (!clicked && clickedElement == nullptr) {
                        clickedElement = currentHoveredElement;
                        clicked = true;
                        if (focusedElement != nullptr) focusedElement->focus(false);
                        focusedElement = clickedElement;
                        if (focusedElement != nullptr) focusedElement->focus(true);
                        setElementsModifierState("clicked", clickedElement, true, SDL_Event{SDL_EVENT_MOUSE_BUTTON_DOWN});
                    }
                }
                else {
                    clicked = false;
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

            void UIManager::sendEvent(const SDL_Event &event, UiElement *leafElement) {
                UiElement *element = leafElement;
                if (element == nullptr) return;
                while (element != nullptr) {
                    element->catchEvent(event);
                    element = element->getParent();
                }
            }

            void UIManager::setElementsModifierState(const std::string &modifier, UiElement *leafElement, bool enabled, const SDL_Event &event) {
                UiElement *element = leafElement;
                if (element == nullptr) return;
                while (element != nullptr) {
                    element->setModifierState(modifier, enabled);
                    if (enabled) element->catchEvent(event);
                    element = element->getParent();
                }
                needUpdate(true);
                // TODO: don't recalculate the entire tree
                // TODO: recalculate and re-render only when an element has changed
            }

        } // namespace manager
    } // namespace element
} // namespace gui
