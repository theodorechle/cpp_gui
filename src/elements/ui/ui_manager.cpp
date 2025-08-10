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
                    prepareRenderNodes(currentElement->getChild(), currentRenderNode);
                    currentElement = currentElement->getNext();
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
                // std::cerr << "computeElementsLayout\n";
                if (rootRenderNode == nullptr) return;
                // std::cerr << "rootRenderNode is not null\n";
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
                // std::cerr << "Rendering elements (renderedTexture is null: " << (renderedTexture == nullptr) << ")\n";
                if (renderedTexture != nullptr) {
                    // std::cerr << "renderedTexture: w=" << renderedTexture->w << ", h=" << renderedTexture->h << "\n";
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
                ui::render::UiRenderNode *currentRenderNode = rootRenderNode->child();
                UiElement *currentElement = nullptr;
                UiElement *currentHoveredElement = nullptr;
                SDL_Rect currentElementRect = SDL_Rect();
                int currentX = 0;
                int currentY = 0;
                // SDL_Delay(10);
                // std::cerr << "start\n";
                // std::cerr << "point: x=" << mousePos.x << ", y=" << mousePos.y << "\n";
                while (currentRenderNode != nullptr) {
                    currentElement = currentRenderNode->baseElement;
                    currentX = currentElementRect.x;
                    currentY = currentElementRect.y;
                    currentElementRect = *currentRenderNode->elementClippedRect(); // copy
                    const ui::Pos pos = *currentRenderNode->startCoords();
                    // std::cerr << "currentElementRect: " << currentElementRect.x << ", " << currentElementRect.y << "\n";
                    // std::cerr << "currentX: " << currentX << ", currentY: " << currentY << "\n";
                    // std::cerr << "pos: " << pos.x << ", " << pos.y << "\n";
                    currentElementRect.x += pos.x + currentX;
                    currentElementRect.y += pos.y + currentY;
                    // std::cerr
                    //     << "element rect ("
                    //     << currentElement->name()
                    //     << "): x="
                    //     << currentElementRect.x
                    //     << ", y="
                    //     << currentElementRect.y
                    //     << ", w="
                    //     << currentElementRect.w
                    //     << ", h="
                    //     << currentElementRect.h
                    //     << "\n";
                    // std::cerr << "hovered: " << (SDL_PointInRect(&mousePos, &currentElementRect) ? "true" : "false") << "\n";
                    if (SDL_PointInRect(&mousePos, &currentElementRect)) {
                        // if (currentElement->name() == "label") {
                        //     std::cerr
                        //         << "hovered element is label with text: -- "
                        //         << static_cast<gui::element::Label *>(currentElement)->getText()
                        //         << " --\n";
                        // }
                        currentElement->displayStyle();
                        currentHoveredElement = currentElement;
                        currentRenderNode = currentRenderNode->child();
                    }
                    else {
                        currentElementRect.x -= pos.x;
                        currentElementRect.y -= pos.y;
                        currentRenderNode = currentRenderNode->next();
                    }
                }
                if (mouseFlags) {
                    if (!clicked && clickedElement == nullptr) {
                        clickedElement = currentHoveredElement;
                        clicked = true;
                        if (focusedElement != nullptr) focusedElement->focus(false);
                        focusedElement = clickedElement;
                        if (focusedElement != nullptr) focusedElement->focus(true);
                        // SDL_Log("clickedElement: %s", clickedElement->name().c_str());
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
