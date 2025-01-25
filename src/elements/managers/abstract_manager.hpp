#ifndef ABSTRACT_MANAGER_HPP
#define ABSTRACT_MANAGER_HPP

#include <algorithm>
#include <list>

#include "../abstract_element.hpp"

namespace gui {
    namespace element {
        namespace manager {

            class AbstractManager {
            public:
                enum class Status {
                    RUNNING,
                    ENDED,
                    ERROR,
                };

            private:
                Status _currentStatus = Status::RUNNING;
                bool _needRendering = true;
                virtual void renderElements() const = 0;

            protected:
                gui::element::AbstractElement *elementsTree = nullptr;
                void status(Status s) { _currentStatus = s; }
                void needRendering(bool needRendering) { this->_needRendering = needRendering; }

            public:
                virtual ~AbstractManager();
                void setElementsTree(gui::element::AbstractElement *element);
                void deleteElementsTree();
                void render();

                Status status() { return _currentStatus; }
                bool needRendering() { return _needRendering; }
            };

        } // namespace manager
    } // namespace element
} // namespace gui

#endif // ABSTRACT_MANAGER_HPP
