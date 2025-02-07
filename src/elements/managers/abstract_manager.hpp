#ifndef ABSTRACT_MANAGER_HPP
#define ABSTRACT_MANAGER_HPP

#include <algorithm>
#include <list>

#include "../abstract_element.hpp"
#include "manager_actions_service.hpp"

namespace gui {
    namespace element {
        namespace manager {

            class AbstractManager {
                gui::element::ManagerActionsService managerActionsService;

            public:
                enum class Status {
                    RUNNING,
                    ENDED,
                    ERROR,
                };

            private:
                Status _currentStatus = Status::RUNNING;
                bool _needRendering = true;
                bool _needRecomputeLayout = true;
                virtual void renderElements() const = 0;

            protected:
                gui::element::AbstractElement *elementsTree = nullptr;
                void status(Status s) { _currentStatus = s; }
                void needRecompute(bool needRecompute) { _needRecomputeLayout = needRecompute; }
                void needRendering(bool needRendering) { _needRendering = needRendering; }
                virtual void computeElementsLayout() = 0;
            public:
                AbstractManager() : managerActionsService{ManagerActionsService(this)} {}
                virtual ~AbstractManager();
                virtual void setElementsTree(gui::element::AbstractElement *element);
                void deleteElementsTree();
                void render();
                virtual void computeLayout();
                ManagerActionsService *getRedrawRequester() { return &managerActionsService; }

                Status status() { return _currentStatus; }
                void askRendering() { this->_needRendering = true; }
                bool needRendering() { return _needRendering; }
                void askRecomputeLayout() { this->_needRecomputeLayout = true; }
                bool needRecompute() { return _needRecomputeLayout; }
            };

        } // namespace manager
    } // namespace element
} // namespace gui

#endif // ABSTRACT_MANAGER_HPP
