#include "ui_manager.hpp"
#include "label.hpp"

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
                UiElement *rootElement = new RootElement();
                rootElement->setRenderer(renderer);
                rootElement->setWindow(window);
                rootElement->manager(this);
                elementsTree = rootElement;
                rootRenderNode = new ui::render::UiRenderNode(renderer, nullptr, rootElement);
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

            void UIManager::addChildToRootElement(AbstractElement *childElement) {
                static_cast<UiElement *>(elementsTree)->addChild(static_cast<UiElement *>(childElement));
            }

            void UIManager::resetEvents() {
                clickedElement = nullptr;
                hoveredElement = nullptr;
                focusedElement = nullptr;
                needUpdate(true);
            }

            void UIManager::computeNodesLayout(ui::render::UiRenderNode *renderNode) {
                renderNode->computeSelfLayout();
                ui::render::UiRenderNode *child = renderNode->child();
                while (child != nullptr) {
                    computeNodesLayout(child);
                    child = child->next();
                }
            }

            void UIManager::initElementsBeforeLayoutComputing(ui::render::UiRenderNode *rootRenderNode) {
                rootRenderNode->initBeforeLayoutComputing();
            }

            void UIManager::restoreAfterLayoutComputing(ui::render::UiRenderNode *rootRenderNode) { rootRenderNode->restoreAfterLayoutComputing(); }

            void UIManager::prepareRenderNodes(UiElement *rootElement, ui::render::UiRenderNode *rootRenderNode, bool isRoot) {
                UiElement *currentElement = rootElement;
                ui::render::UiRenderNode *currentRenderNode;
                while (currentElement != nullptr) {
                    if (isRoot) {
                        currentRenderNode = rootRenderNode;
                    }
                    else {
                        currentRenderNode = ui::render::elementToRenderNodeConverter(renderer, rootRenderNode, currentElement);
                        rootRenderNode->addChild(currentRenderNode);
                    }
                    prepareRenderNodes(currentElement->child(), currentRenderNode);
                    currentElement = currentElement->next();
                }
            }

            void UIManager::computeNodesAndChildsLayout(ui::render::UiRenderNode *node) { node->computeSelfAndChildsLayout(); }

            void UIManager::computeNodesRelativeLayout(ui::render::UiRenderNode *node) {
                ui::render::UiRenderNode *currentNode = node;
                while (currentNode != nullptr) {
                    currentNode->computeRelativeLayout();
                    currentNode = currentNode->next();
                }
            }

            void UIManager::computeNodesFinalLayout(ui::render::UiRenderNode *node, SDL_Rect *rootClipRect) {
                if (node == nullptr) return;
                if (rootClipRect == nullptr) node->computeFinalLayout();
                else node->computeFinalLayout(rootClipRect, true);
            }

            void UIManager::computeElementsLayout() {
                if (rootRenderNode == nullptr) return;
                delete rootRenderNode->child(); // TODO: remove
                rootRenderNode->removeChilds();
                prepareRenderNodes(static_cast<UiElement *>(elementsTree), rootRenderNode, true);
                initElementsBeforeLayoutComputing(rootRenderNode);
                computeNodesLayout(rootRenderNode);
                computeNodesAndChildsLayout(rootRenderNode);
                computeNodesRelativeLayout(rootRenderNode);
                computeNodesFinalLayout(rootRenderNode, &clipRect);
                restoreAfterLayoutComputing(rootRenderNode);
            }

            void UIManager::createNodesTextures(ui::render::UiRenderNode *node) {
                ui::render::UiRenderNode *currentNode = node;
                while (currentNode != nullptr) {
                    currentNode->createTexture();
                    createNodesTextures(currentNode->child());
                    currentNode = currentNode->next();
                }
            }

            void UIManager::createRenderedTexture() {
                if (!needRenderingUpdate) return;
                // needRenderingUpdate = false; // TODO: uncomment
                if (rootRenderNode == nullptr) return;
                createNodesTextures(rootRenderNode);
                SDL_SetRenderTarget(renderer, renderedTexture);
                rootRenderNode->render();
                SDL_SetRenderTarget(renderer, nullptr);
            }

            void UIManager::renderElements(bool clear) const {
                if (renderedTexture != nullptr) {
                    SDL_RenderTexture(renderer, renderedTexture, nullptr, nullptr);
                }
                SDL_RenderPresent(renderer);
            }

            void UIManager::update() {
                computeElementsLayout(); // TODO: improve to only render updated elements
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
                case SDL_EVENT_WINDOW_MOUSE_LEAVE:
                    windowFocused = false;
                    break;
                case SDL_EVENT_WINDOW_MOUSE_ENTER:
                    windowFocused = true;
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
                ui::render::UiRenderNode *currentRenderNode = rootRenderNode->child();
                UiElement *currentElement = nullptr;
                UiElement *currentHoveredElement = nullptr;
                SDL_Rect currentElementRect = SDL_Rect();
                int currentX = 0;
                int currentY = 0;
                if (windowFocused) {
                    while (currentRenderNode != nullptr) {
                        currentElement = currentRenderNode->baseElement;
                        currentX = currentElementRect.x;
                        currentY = currentElementRect.y;
                        currentElementRect = *currentRenderNode->elementClippedRect(); // copy
                        const ui::Pos pos = *currentRenderNode->startCoords();
                        currentElementRect.x += pos.x + currentX;
                        currentElementRect.y += pos.y + currentY;
                        if (SDL_PointInRect(&mousePos, &currentElementRect)) {
                            currentHoveredElement = currentElement;
                            currentRenderNode = currentRenderNode->child();
                        }
                        else {
                            currentElementRect.x -= pos.x;
                            currentElementRect.y -= pos.y;
                            currentRenderNode = currentRenderNode->next();
                        }
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
                if ((hoveredElement != currentHoveredElement) || (hoveredElement && !windowFocused)) {
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
                    element = element->parent();
                }
            }

            void UIManager::setElementsModifierState(const std::string &modifier, UiElement *leafElement, bool enabled, const SDL_Event &event) {
                UiElement *element = leafElement;
                if (element == nullptr) return;
                while (element != nullptr) {
                    element->setModifierState(modifier, enabled);
                    if (enabled) element->catchEvent(event);
                    element = element->parent();
                }
                needUpdate(true);
            }

        } // namespace manager
    } // namespace element
} // namespace gui
