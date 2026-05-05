#include "ui_manager.hpp"
#include "root_element.hpp"
#include "utils.hpp"
#include <SDL3/SDL_events.h>

namespace gui::element {
    namespace manager {
        UiManager::UiManager(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect *clipRect) : window{window}, renderer{renderer} {
            if (clipRect != nullptr) {
                this->clipRect = *clipRect;
                canChangeSize = false;
            }
            else {
                updateRenderingData();
            }

            registeredSdlEventHandlers.insert({SDL_EVENT_QUIT, [this](const SDL_Event *event) { this->status(Status::ENDED); }});
            registeredSdlEventHandlers.insert({SDL_EVENT_WINDOW_RESIZED, [this](const SDL_Event *event) { this->refreshAll(); }});
            registeredSdlEventHandlers.insert({SDL_EVENT_WINDOW_FOCUS_GAINED, [this](const SDL_Event *event) { this->windowFocusGained(); }});
            registeredSdlEventHandlers.insert({SDL_EVENT_WINDOW_MOUSE_ENTER, [this](const SDL_Event *event) { this->windowFocusGained(); }});
            registeredSdlEventHandlers.insert({SDL_EVENT_WINDOW_FOCUS_LOST, [this](const SDL_Event *event) { this->windowFocusLost(); }});
            registeredSdlEventHandlers.insert({SDL_EVENT_WINDOW_MOUSE_LEAVE, [this](const SDL_Event *event) { this->windowFocusLost(); }});
            registeredSdlEventHandlers.insert({SDL_EVENT_MOUSE_MOTION, [this](const SDL_Event *event) { this->processMouseEvent(event); }});
            registeredSdlEventHandlers.insert({SDL_EVENT_MOUSE_BUTTON_DOWN, [this](const SDL_Event *event) { this->processMouseEvent(event); }});
            registeredSdlEventHandlers.insert({SDL_EVENT_MOUSE_BUTTON_UP, [this](const SDL_Event *event) { this->processMouseEvent(event); }});
            registeredSdlEventHandlers.insert(
                {SDL_EVENT_MOUSE_WHEEL, [this](const SDL_Event *event) {
                     if (hoveredElement) {
                         const ui::event::MouseWheelEvent guiEvent = ui::event::MouseWheelEvent{ui::event::GuiEventType::EVENT_SCROLL,
                                                                                                event->wheel.mouse_x,
                                                                                                event->wheel.mouse_y,
                                                                                                event->button.button,
                                                                                                static_cast<float>(event->wheel.integer_x),
                                                                                                static_cast<float>(event->wheel.integer_y)};
                         sendEvent(&guiEvent, hoveredElement->baseElement());
                         scroll(guiEvent.scrollX, guiEvent.scrollY); // TODO: elements should be able to intercept the event (canvas for example)
                     }
                 }});
            registeredSdlEventHandlers.insert({SDL_EVENT_KEY_DOWN, [this](const SDL_Event *event) {
                                                   this->sendEventToUiRenderNodeElement(ui::event::KeyEvent{ui::event::GuiEventType::EVENT_KEY_DOWN,
                                                                                                            event->key.scancode, event->key.key,
                                                                                                            event->key.mod},
                                                                                        focusedElement);
                                               }});
            registeredSdlEventHandlers.insert({SDL_EVENT_KEY_UP, [this](const SDL_Event *event) {
                                                   this->sendEventToUiRenderNodeElement(ui::event::KeyEvent{ui::event::GuiEventType::EVENT_KEY_UP,
                                                                                                            event->key.scancode, event->key.key,
                                                                                                            event->key.mod},
                                                                                        focusedElement);
                                               }});
            registeredSdlEventHandlers.insert({SDL_EVENT_TEXT_INPUT, [this](const SDL_Event *event) {
                                                   this->sendEventToUiRenderNodeElement(
                                                       ui::event::TextEvent{ui::event::GuiEventType::EVENT_TEXT_INPUT, event->text.text},
                                                       focusedElement);
                                               }});
        }

        UiManager::~UiManager() {
            delete rootRenderNode;
            SDL_DestroyTexture(renderedTexture);
        }

        void UiManager::createRootElement() {
            UiElement *rootElement = new RootElement();
            rootElement->renderer(renderer);
            rootElement->window(window);
            rootElement->manager(this);
            elementsTree = rootElement;
            rootRenderNode = new ui::render::UiRenderNode(renderer, nullptr, rootElement);
        }

        void UiManager::updateRenderingData() {
            if (!canChangeSize) return;
            int width = 0;
            int height = 0;
            if (!SDL_GetCurrentRenderOutputSize(renderer, &width, &height)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UiManager::updateRenderingData: Can't get render size");
            }
            this->clipRect = SDL_Rect{0, 0, width, height};
            renderedTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, clipRect.w, clipRect.h);
            if (renderedTexture == nullptr) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "UiManager::updateRenderingData: Can't create a texture for rendering: %s", SDL_GetError());
            }
        }

        void UiManager::addChildToRootElement(AbstractElement *childElement) {
            static_cast<UiElement *>(elementsTree)->addChild(static_cast<UiElement *>(childElement));
        }

        ui::render::UiRenderNode *UiManager::renderNodeOf(const AbstractElement *element) {
            if (!element) return nullptr;
            if (element == rootRenderNode->baseElement()) return rootRenderNode;
            std::list<const gui::element::AbstractElement *> pathElement;
            while (element) {
                pathElement.push_front(element);
                element = element->parent();
            }

            pathElement.pop_front(); // no need of root since it has already been checked

            ui::render::UiRenderNode *renderNode = rootRenderNode;
            for (const gui::element::AbstractElement *pathFragment : pathElement) {
                renderNode = renderNode->child();
                if (!renderNode) return nullptr;
                while (renderNode->baseElement() != pathFragment) {
                    renderNode = renderNode->next();
                }
            }
            return renderNode;
        }

        void UiManager::resetInvalidPointersOnNodesDeletion(const ui::render::UiRenderNode *parentNode, bool deleteUpdateElement) {
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

        ui::render::UiRenderNode *UiManager::hoveredLeafElement(SDL_Point *coordinates) {
            ui::render::UiRenderNode *renderNode = rootRenderNode;
            SDL_Rect rect;
            ui::Pos pos;
            ui::Pos absolutePos = {0, 0};

            while (true) {
                rect = *renderNode->elementClippedRect();
                pos = *renderNode->startCoords();
                rect.x = absolutePos.x + pos.x;
                rect.y = absolutePos.y + pos.y;
#ifdef DEBUG
                std::clog
                    << "render node rect: x="
                    << rect.x
                    << ", y="
                    << rect.y
                    << ", w="
                    << rect.w
                    << ", h="
                    << rect.h
                    << "; "
                    << renderNode
                    << " (start coords: "
                    << pos.x
                    << ", "
                    << pos.y
                    << ")"
                    << "\n";
#endif
                if (SDL_PointInRect(coordinates, &rect)) {
#ifdef DEBUG
                    std::cerr << "choosed\n";
#endif
                    absolutePos.x += pos.x;
                    absolutePos.y += pos.y;
                    if (!renderNode->child()) return renderNode;
                    renderNode = renderNode->child();
                }
                else {
                    if (!renderNode->next()) return renderNode->parent() ? renderNode->parent() : renderNode;
                    renderNode = renderNode->next();
                }
            }
        }

        void UiManager::sendEventToUiRenderNodeElement(const event::Event &&event, ui::render::UiRenderNode *leafNode) {
            if (leafNode != nullptr) {
                sendEvent(&event, leafNode->baseElement());
            }
        }

        void UiManager::processMouseEvent(const SDL_Event *sdlEvent) { // TODO: split into different functions for each mouse event
            if (elementsTree == nullptr || !windowFocused) return;
            float x, y;
            SDL_MouseButtonFlags mouseFlags = SDL_GetMouseState(&x, &y);
            SDL_Point mousePos = SDL_Point{static_cast<int>(x), static_cast<int>(y)};

            ui::render::UiRenderNode *hoveredRenderNode = hoveredLeafElement(&mousePos);
#ifdef DEBUG
            std::clog
                << "mouse pos: ("
                << mousePos.x
                << ", "
                << mousePos.y
                << "), hovered leaf render node: "
                << hoveredRenderNode->debugValue()
                << "; element: "
                << hoveredRenderNode->baseElement()->debugValue()
                << "\n";
#endif

            const ui::Pos *startCoords = hoveredRenderNode->startCoords();

            const ui::FPos relativeCoordinates = {static_cast<float>(mousePos.x - startCoords->x), static_cast<float>(mousePos.y - startCoords->y)};

            if (sdlEvent->type == SDL_EVENT_MOUSE_MOTION && hoveredElement) {
                ui::event::MouseMotionEvent event = ui::event::MouseMotionEvent{ui::event::GuiEventType::EVENT_MOUSE_MOTION, relativeCoordinates.x,
                                                                                relativeCoordinates.y, mouseFlags};
                sendEvent(&event, hoveredElement->baseElement());
            }

            // send events to elements and update clicked/hovered/focused pointers
            if (mouseFlags) {
                if (!clicked && !clickedElement) {
                    clickedElement = hoveredRenderNode;
                    clicked = true;
                    if (focusedElement) {
                        event::Event event = event::Event{ui::event::GuiEventType::EVENT_FOCUS_LOST};
                        setElementsModifierState("focused", focusedElement->baseElement(), false, &event);
                    }
                    focusedElement = clickedElement;
                    if (focusedElement) {
                        event::Event event = event::Event{ui::event::GuiEventType::EVENT_FOCUS_GAINED};
                        setElementsModifierState("focused", focusedElement->baseElement(), true, &event);
                    }
                    if (clickedElement) {
                        ui::render::UiRenderNode *element = clickedElement;
                        while (element != nullptr) {
                            ui::event::MouseEvent event = ui::event::MouseEvent{
                                ui::event::GuiEventType::EVENT_MOUSE_BUTTON_DOWN, relativeCoordinates.x - element->startCoords()->x,
                                relativeCoordinates.y - element->startCoords()->y, sdlEvent->button.button};
                            element->baseElement()->setModifierState("clicked", true);
                            element->baseElement()->updateStyle();
                            element->baseElement()->catchEvent(&event);
                            element = element->parent();
                        }
                    }
                }
            }
            else {
                clicked = false;
                if (clickedElement) {
                    ui::event::MouseEvent event =
                        ui::event::MouseEvent{ui::event::GuiEventType::EVENT_MOUSE_BUTTON_UP, 0, 0, sdlEvent->button.button};
                    setElementsModifierState("clicked", clickedElement->baseElement(), false, &event);
                    clickedElement = nullptr;
                }
            }
            if (hoveredElement != hoveredRenderNode) {
                if (hoveredElement) {
                    ui::event::MouseEvent event = ui::event::MouseEvent{ui::event::GuiEventType::EVENT_UNHOVER, 0, 0, 0};
                    setElementsModifierState("hovered", hoveredElement->baseElement(), false, &event);
                }
                if (hoveredRenderNode) {
                    ui::event::MouseEvent event = ui::event::MouseEvent{ui::event::GuiEventType::EVENT_HOVER, relativeCoordinates.x,
                                                                        relativeCoordinates.y, sdlEvent->button.button};
                    setElementsModifierState("hovered", hoveredRenderNode->baseElement(), true, &event);
                }
                hoveredElement = hoveredRenderNode;
            }
        }

        void UiManager::elementEvent(event::ElementEvent event, AbstractElement *caller) {
            AbstractManager::elementEvent(event, caller);
            switch (event) {
            case event::ElementEvent::REMOVE_CHILDS: {
                ui::render::UiRenderNode *node = renderNodeOf(caller);
                resetInvalidPointersOnNodesDeletion(node);
                if (node) {
                    delete node->child();
                    node->removeChilds(); // TODO: remove only concerned child
                }
                needUpdate(elementsTree);
                break;
            }
            case event::ElementEvent::ADD_CHILD:
                needUpdate(elementsTree);
                break;
            default:
                break;
            }
        }

        void UiManager::computeNodesLayout(ui::render::UiRenderNode *renderNode) {
            renderNode->computeSelfLayout();
            ui::render::UiRenderNode *child = renderNode->child();
            while (child != nullptr) {
                computeNodesLayout(child);
                child = child->next();
            }
        }

        void UiManager::initElementsBeforeLayoutComputing(ui::render::UiRenderNode *rootRenderNode) { rootRenderNode->initBeforeLayoutComputing(); }

        void UiManager::restoreAfterLayoutComputing(ui::render::UiRenderNode *rootRenderNode) { rootRenderNode->restoreAfterLayoutComputing(); }

        void UiManager::prepareRenderNodes(UiElement *rootElement, ui::render::UiRenderNode *rootRenderNode) {
            UiElement *currentElement = rootElement;
            ui::render::UiRenderNode *currentRenderNode;
            while (currentElement) {
                currentRenderNode = new ui::render::UiRenderNode(renderer, rootRenderNode, currentElement);
                rootRenderNode->addChild(currentRenderNode);
                prepareRenderNodes(static_cast<UiElement *>(currentElement->child()), currentRenderNode);
                currentElement = static_cast<UiElement *>(currentElement->next());
            }
        }

        void UiManager::computeNodesAndChildsLayout(ui::render::UiRenderNode *node) { node->computeSelfAndChildsLayout(); }

        void UiManager::computeNodesRelativeLayout(ui::render::UiRenderNode *node) {
            ui::render::UiRenderNode *currentNode = node;
            while (currentNode) {
                currentNode->computeRelativeLayout();
                currentNode = currentNode->next();
            }
        }

        void UiManager::computeNodesFinalLayout(ui::render::UiRenderNode *node, SDL_Rect *rootClipRect) {
            if (!node) return;
            if (!rootClipRect) node->computeFinalLayout();
            else node->computeFinalLayout(rootClipRect, true);
        }

        void UiManager::updateModifiedElements() {
            if (!elementsToUpdate.size()) return;
#ifdef DEBUG
            std::clog << "hovered element: " << hoveredElement << "\n";
            std::clog << "clicked element: " << clickedElement << "\n";
            std::clog << "focused element: " << focusedElement << "\n";
#endif
#ifdef DEBUG_DEBUG
            std::clog << "ui_elements\n";
            elementsTree->debugDisplay();
            std::clog << "render_nodes\n";
            rootRenderNode->debugDisplay();
#endif
            const UiElement *hoveredUiElement = hoveredElement ? hoveredElement->baseElement() : nullptr;
            const UiElement *clickedUiElement = clickedElement ? clickedElement->baseElement() : nullptr;
            const UiElement *focusedUiElement = focusedElement ? focusedElement->baseElement() : nullptr;
            for (AbstractElement *elementToUpdate : elementsToUpdate) {
                ui::render::UiRenderNode *renderNode = renderNodeOf(elementToUpdate);
#ifdef DEBUG
                std::clog << "element to refresh: " << renderNode << "\n";
#endif
                if (!renderNode) continue;
                delete renderNode->child();
                renderNode->removeChilds();
#ifdef DEBUG_DEBUG
                std::clog << "render_nodes after removing\n";
                rootRenderNode->debugDisplay();
#endif
                prepareRenderNodes(static_cast<UiElement *>(elementToUpdate->child()), renderNode);
#ifdef DEBUG_DEBUG
                std::clog << "render_nodes after preparation\n";
                rootRenderNode->debugDisplay();
#endif
            }
            initElementsBeforeLayoutComputing(rootRenderNode);
            hoveredElement = renderNodeOf(hoveredUiElement);
            clickedElement = renderNodeOf(clickedUiElement);
            focusedElement = renderNodeOf(focusedUiElement);
#ifdef DEBUG
            std::clog << "hovered element after update: " << hoveredElement << "\n";
            std::clog << "clicked element after update: " << clickedElement << "\n";
            std::clog << "focused element after update: " << focusedElement << "\n";
#endif
        }

        void UiManager::computeElementsLayout() {
            computeNodesLayout(rootRenderNode);
            computeNodesAndChildsLayout(rootRenderNode);
            computeNodesRelativeLayout(rootRenderNode);
            computeNodesFinalLayout(rootRenderNode, &clipRect);
            restoreAfterLayoutComputing(rootRenderNode);
        }

        void UiManager::createNodesTextures(ui::render::UiRenderNode *node) {
            ui::render::UiRenderNode *currentNode = node;
            while (currentNode != nullptr) {
                currentNode->createTexture();
                createNodesTextures(currentNode->child());
                currentNode = currentNode->next();
            }
        }

        void UiManager::createRenderedTexture() {
            if (!elementsToUpdate.size()) return;
            createNodesTextures(rootRenderNode);
            SDL_SetRenderTarget(renderer, renderedTexture);
            rootRenderNode->render();
            SDL_SetRenderTarget(renderer, nullptr);
        }

        void UiManager::renderElements(bool clear) const {
            if (renderedTexture != nullptr) {
                SDL_RenderTexture(renderer, renderedTexture, nullptr, nullptr);
            }
            SDL_RenderPresent(renderer);
        }

        void UiManager::update() {
            AbstractManager::update();
            computeElementsLayout(); // TODO: improve to only render updated elements
            createRenderedTexture();
        }

        void UiManager::windowFocusGained() { this->windowFocused = true; }

        void UiManager::windowFocusLost() {
            this->windowFocused = false;
            if (focusedElement) {
                event::Event event = event::Event{ui::event::GuiEventType::EVENT_FOCUS_LOST};
                setElementsModifierState("focused", focusedElement->baseElement(), false, &event);
                focusedElement = nullptr;
            }
            if (clickedElement) {
                ui::event::MouseEvent event = ui::event::MouseEvent{ui::event::GuiEventType::EVENT_HOVER, 0, 0};
                setElementsModifierState("clicked", clickedElement->baseElement(), false, &event);
                clickedElement = nullptr;
            }
            if (hoveredElement) {
                ui::event::MouseEvent event = ui::event::MouseEvent{ui::event::GuiEventType::EVENT_HOVER, 0, 0};
                setElementsModifierState("hovered", hoveredElement->baseElement(), false, &event);
                hoveredElement = nullptr;
            }
#ifdef DEBUG
            std::clog << "window focus lost, removed events\n";
#endif
        }

        void UiManager::refreshAll() {
            updateRenderingData();
            needUpdate(elementsTree);
        }

        void UiManager::processEvent(const SDL_Event *sdlEvent) {
            // TODO: elements should be able to stop the propagation of some events (for example scroll (canvas))
            std::map<Uint32, EventHandler>::const_iterator it = registeredSdlEventHandlers.find(sdlEvent->type);
            if (it != registeredSdlEventHandlers.cend()) it->second(sdlEvent);
        }

        void UiManager::scroll(int x, int y) {
            ui::render::UiRenderNode *element = hoveredElement;
            while (element) {
                if (element->scroll(x, -y)) {
                    needUpdate(element->baseElement());
                    return;
                }
                element = element->parent();
            }
        }
    } // namespace manager
} // namespace gui::element
