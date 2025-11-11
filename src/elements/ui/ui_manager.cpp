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

            ui::render::UiRenderNode *UIManager::renderNodeOf(const AbstractElement *element) {
                if (!element) return nullptr;
                if (element == rootRenderNode->baseElement) return rootRenderNode;
                const AbstractElement *node = element; // TODO: should be useless
                std::list<const gui::element::AbstractElement *> pathElement;
                while (node) {
                    pathElement.push_front(node);
                    node = node->parent();
                }

                pathElement.pop_front(); // no need of root since it has already been checked

                ui::render::UiRenderNode *renderNode = rootRenderNode;
                for (const gui::element::AbstractElement *pathFragment : pathElement) {
                    renderNode = renderNode->child();
                    if (!renderNode) return nullptr;
                    while (renderNode->baseElement != pathFragment) {
                        renderNode = renderNode->next();
                    }
                }
                return renderNode;
            }

            void UIManager::resetInvalidPointersOnNodesDeletion(const ui::render::UiRenderNode *parentNode, bool deleteUpdateElement) {
                if (!parentNode) return;
                if (clickedElement && (parentNode->isParentOf(clickedElement))) clickedElement = nullptr;
                if (hoveredElement && (parentNode->isParentOf(hoveredElement))) hoveredElement = nullptr;
                if (focusedElement && (parentNode->isParentOf(focusedElement))) focusedElement = nullptr;
                if (deleteUpdateElement) {
                    for (std::set<gui::element::AbstractElement *>::iterator it = elementsToUpdate.begin(); it != elementsToUpdate.end();) {
                        const ui::render::UiRenderNode *updateNode = renderNodeOf(*it);
                        if ((parentNode->isParentOf(updateNode))) it = elementsToUpdate.erase(it);
                        else it++;
                    }
                }
            }

            void UIManager::elementEvent(ElementEvent event, AbstractElement *caller) {
                AbstractManager::elementEvent(event, caller);
                switch (event) {
                case ElementEvent::REMOVE_CHILDS: {
                    ui::render::UiRenderNode *node = renderNodeOf(caller);
                    resetInvalidPointersOnNodesDeletion(node);
                    if (node) {
                        delete node->child();
                        node->removeChilds(); // TODO: remove only concerned child
                    }
                    needUpdate(elementsTree);
                    break;
                }
                case ElementEvent::ADD_CHILD:
                    needUpdate(elementsTree);
                    break;
                default:
                    break;
                }
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

            void UIManager::prepareRenderNodes(UiElement *rootElement, ui::render::UiRenderNode *rootRenderNode) {
                UiElement *currentElement = rootElement;
                ui::render::UiRenderNode *currentRenderNode;
                while (currentElement) {
                    currentRenderNode = new ui::render::UiRenderNode(renderer, rootRenderNode, currentElement);
                    rootRenderNode->addChild(currentRenderNode);
                    prepareRenderNodes(static_cast<UiElement *>(currentElement->child()), currentRenderNode);
                    currentElement = static_cast<UiElement *>(currentElement->next());
                }
            }

            void UIManager::computeNodesAndChildsLayout(ui::render::UiRenderNode *node) { node->computeSelfAndChildsLayout(); }

            void UIManager::computeNodesRelativeLayout(ui::render::UiRenderNode *node) {
                ui::render::UiRenderNode *currentNode = node;
                while (currentNode) {
                    currentNode->computeRelativeLayout();
                    currentNode = currentNode->next();
                }
            }

            void UIManager::computeNodesFinalLayout(ui::render::UiRenderNode *node, SDL_Rect *rootClipRect) {
                if (!node) return;
                if (!rootClipRect) node->computeFinalLayout();
                else node->computeFinalLayout(rootClipRect, true);
            }

            void UIManager::computeElementsLayout() {
                if (!elementsToUpdate.size()) return;
#ifdef DEBUG
                std::cerr << "hovered element: " << hoveredElement << "\n";
                std::cerr << "clicked element: " << clickedElement << "\n";
                std::cerr << "focused element: " << focusedElement << "\n";
                std::cerr << "ui_elements\n";
                elementsTree->debugDisplay();
                std::cerr << "render_nodes\n";
                rootRenderNode->debugDisplay();
#endif
                const UiElement *hoveredUiElement = hoveredElement ? hoveredElement->baseElement : nullptr;
                const UiElement *clickedUiElement = clickedElement ? clickedElement->baseElement : nullptr;
                const UiElement *focusedUiElement = focusedElement ? focusedElement->baseElement : nullptr;
                for (AbstractElement *elementToUpdate : elementsToUpdate) {
                    ui::render::UiRenderNode *renderNode = renderNodeOf(elementToUpdate);
#ifdef DEBUG
                    std::cerr << "element to refresh: " << renderNode << "\n";
#endif
                    if (!renderNode) continue;
                    delete renderNode->child();
                    renderNode->removeChilds();
#ifdef DEBUG

                    std::cerr << "render_nodes after removing\n";
                    rootRenderNode->debugDisplay();
#endif
                    prepareRenderNodes(static_cast<UiElement *>(elementToUpdate->child()), renderNode);
#ifdef DEBUG
                    std::cerr << "render_nodes after preparation\n";
                    rootRenderNode->debugDisplay();
#endif
                }
                initElementsBeforeLayoutComputing(rootRenderNode);
                hoveredElement = renderNodeOf(hoveredUiElement);
                clickedElement = renderNodeOf(clickedUiElement);
                focusedElement = renderNodeOf(focusedUiElement);
#ifdef DEBUG
                std::cerr << "hovered element: " << hoveredElement << "\n";
                std::cerr << "clicked element: " << clickedElement << "\n";
                std::cerr << "focused element: " << focusedElement << "\n";
#endif
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
                if (!elementsToUpdate.size()) return;
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

            void UIManager::processEvent(const SDL_Event *event) {
                switch (event->type) {
                case SDL_EVENT_QUIT:
                    status(Status::ENDED);
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    processMouseEvent(event);
                    break;
                case SDL_EVENT_WINDOW_RESIZED:
                    needUpdate(elementsTree);
                    break;
                case SDL_EVENT_WINDOW_MOUSE_LEAVE:
                    windowFocused = false;
                    break;
                case SDL_EVENT_WINDOW_MOUSE_ENTER:
                    windowFocused = true;
                    break;
                case SDL_EVENT_MOUSE_WHEEL:
                    scroll(event->wheel.x, event->wheel.y);
                    break;
                default:
                    if (focusedElement != nullptr) sendEvent(event, focusedElement->baseElement);
                }
            }

            void UIManager::processMouseEvent(const SDL_Event *event) { // TODO: split into different functions for each mouse events
                if (elementsTree == nullptr) return;
                float x, y;
                SDL_MouseButtonFlags mouseFlags = SDL_GetMouseState(&x, &y);
                SDL_Point mousePos = SDL_Point{(int)x, (int)y};
                ui::render::UiRenderNode *currentRenderNode = rootRenderNode->child();
                ui::render::UiRenderNode *currentElement = nullptr;
                ui::render::UiRenderNode *currentHoveredElement = nullptr;
                SDL_Rect currentElementRect = SDL_Rect();
                int currentX = 0;
                int currentY = 0;
                if (windowFocused) {
                    while (currentRenderNode != nullptr) {
                        currentElement = currentRenderNode;
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
                    if (!clicked && !clickedElement) {
                        clickedElement = currentHoveredElement;
                        clicked = true;
                        if (focusedElement) {
                            SDL_Event event = SDL_Event();
                            SDL_zero(event);
                            event.user.type = ui::FOCUS_LOST;
                            event.user.code = ui::eventCode(event.user.type);
                            setElementsModifierState("focused", focusedElement->baseElement, false, &event);
                        }
                        focusedElement = clickedElement;
                        if (focusedElement) {
                            SDL_Event event = SDL_Event();
                            SDL_zero(event);
                            event.user.type = ui::FOCUS;
                            event.user.code = ui::eventCode(event.user.type);
                            setElementsModifierState("focused", focusedElement->baseElement, true, &event);
                        }
                        if (clickedElement) setElementsModifierState("clicked", clickedElement->baseElement, true, event);
                    }
                }
                else {
                    clicked = false;
                    if (clickedElement) {
                        setElementsModifierState("clicked", clickedElement->baseElement, false, event);
                        clickedElement = nullptr;
                    }
                }
                if ((hoveredElement != currentHoveredElement) || (hoveredElement && !windowFocused)) {
                    if (hoveredElement) setElementsModifierState("hovered", hoveredElement->baseElement, false, event);
                    if (currentHoveredElement) setElementsModifierState("hovered", currentHoveredElement->baseElement, true, event);
                    hoveredElement = currentHoveredElement;
                }
            }

            void UIManager::scroll(int x, int y) { hoveredElement->scroll(x, y); }

            void UIManager::sendEvent(const SDL_Event *event, UiElement *leafElement) {
                UiElement *element = leafElement;
                if (element == nullptr) return;
                while (element != nullptr) {
                    element->catchEvent(event);
                    element = static_cast<UiElement *>(element->parent());
                }
            }

            void UIManager::setElementsModifierState(const std::string &modifier, UiElement *leafElement, bool enabled, const SDL_Event *event) {
                UiElement *element = leafElement;
                while (element != nullptr) {
                    element->setModifierState(modifier, enabled);
                    if (enabled && event) element->catchEvent(event);
                    element = static_cast<UiElement *>(element->parent());
                }
                needUpdate(leafElement);
            }

        } // namespace manager
    } // namespace element
} // namespace gui
