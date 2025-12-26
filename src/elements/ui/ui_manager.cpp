#include "ui_manager.hpp"
#include "label.hpp"

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
            // TODO: some part should be done in abstract manager, at least calling the method
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
            updateModifiedElements();
            computeElementsLayout(); // TODO: improve to only render updated elements
            createRenderedTexture();
        }

        void UiManager::windowFocusLost() {
            if (focusedElement) {
                event::Event event = event::Event{ui::event::EVENT_FOCUS_LOST};
                setElementsModifierState("focused", focusedElement->baseElement, false, &event);
                focusedElement = nullptr;
            }
            if (clickedElement) {
                ui::event::MouseEvent event = ui::event::MouseEvent{ui::event::EVENT_HOVER, 0, 0};
                setElementsModifierState("clicked", clickedElement->baseElement, false, &event);
                clickedElement = nullptr;
            }
            if (hoveredElement) {
                ui::event::MouseEvent event = ui::event::MouseEvent{ui::event::EVENT_HOVER, 0, 0};
                setElementsModifierState("hovered", hoveredElement->baseElement, false, &event);
                hoveredElement = nullptr;
            }
#ifdef DEBUG
            std::cerr << "window focus lost, removed events\n";
#endif
        }

        void UiManager::processEvent(const SDL_Event *event) {
            // TODO: elements should be able to stop the propagation of some events (for example scroll (canvas))
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
            case SDL_EVENT_WINDOW_FOCUS_LOST:
            case SDL_EVENT_WINDOW_MOUSE_LEAVE:
                windowFocused = false;
                windowFocusLost();
                break;
            case SDL_EVENT_WINDOW_FOCUS_GAINED:
            case SDL_EVENT_WINDOW_MOUSE_ENTER:
                windowFocused = true;
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                scroll(event->wheel.x, event->wheel.y); // TODO: elements should be able to intercept the event (canvas for example)
                break;
                // default:
                //     if (focusedElement != nullptr) sendEvent(event, focusedElement->baseElement);
            }
        }

        void UiManager::processMouseEvent(const SDL_Event *event) { // TODO: split into different functions for each mouse events
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
                // search the hovered leaf element
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

            // send events to elements and update clicked/hovered/focused pointers
            if (mouseFlags) {
                if (!clicked && !clickedElement) {
                    clickedElement = currentHoveredElement;
                    clicked = true;
                    if (focusedElement) {
                        event::Event event = event::Event{ui::event::EVENT_FOCUS_LOST};
                        setElementsModifierState("focused", focusedElement->baseElement, false, &event);
                    }
                    focusedElement = clickedElement;
                    if (focusedElement) {
                        event::Event event = event::Event{ui::event::EVENT_FOCUS_GAINED};
                        setElementsModifierState("focused", focusedElement->baseElement, true, &event);
                    }
                    if (clickedElement) {
                        // FIXME: UPDATE COORDS FOR EACH ELEMENT
                        // for now, every element who get the event EXCEPT the leaf is receiving wrong coords
                        // would be better if it translate to relative coords only if element asks for it
                        // (no need to translate for every element)
                        // but the element would know his place in the window (is it harmful, I don't know)
                        ui::event::MouseEvent event = ui::event::MouseEvent{
                            ui::event::EVENT_MOUSE_BUTTON_DOWN, static_cast<float>(mousePos.x - currentX), static_cast<float>(mousePos.y - currentY)};
                        setElementsModifierState("clicked", clickedElement->baseElement, true, &event);
                    }
                }
            }
            else {
                clicked = false;
                if (clickedElement) {
                    ui::event::MouseEvent event = ui::event::MouseEvent{ui::event::EVENT_MOUSE_BUTTON_UP, 0, 0};
                    setElementsModifierState("clicked", clickedElement->baseElement, false, &event);
                    clickedElement = nullptr;
                }
            }
            if (hoveredElement != currentHoveredElement) {
                if (hoveredElement) {
                    ui::event::MouseEvent event = ui::event::MouseEvent{ui::event::EVENT_UNHOVER, 0, 0};
                    setElementsModifierState("hovered", hoveredElement->baseElement, false, &event);
                }
                if (currentHoveredElement) {
                    ui::event::MouseEvent event = ui::event::MouseEvent{ui::event::EVENT_HOVER, 0, 0};
                    setElementsModifierState("hovered", currentHoveredElement->baseElement, true, &event);
                }
                hoveredElement = currentHoveredElement;
            }
        }

        void UiManager::scroll(int x, int y) {
            ui::render::UiRenderNode *element = hoveredElement;
            while (element) {
                if (element->scroll(x, -y)) {
                    needUpdate(element->baseElement);
                    return;
                }
                element = element->parent();
            }
        }

        void UiManager::sendEvent(const event::Event *event, UiElement *leafElement) {
            UiElement *element = leafElement;
            if (element == nullptr) return;
            while (element != nullptr) {
                element->catchEvent(event);
                element = static_cast<UiElement *>(element->parent());
            }
        }

    } // namespace manager
} // namespace gui::element
