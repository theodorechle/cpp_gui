#ifndef ABSTRACT_MANAGER_HPP
#define ABSTRACT_MANAGER_HPP

#include <algorithm>
#include <list>

#include "abstract_element.hpp"

namespace gui::element::manager {
    class AbstractManager {
    public:
        enum class Status {
            RUNNING,
            ENDED,
            ERROR,
        };

    private:
        Status _currentStatus = Status::RUNNING;
        bool _needUpdate = true;
        virtual void renderElements(bool clear = true) const = 0;
        virtual void createRootElement() = 0;
        virtual void update() = 0;
        virtual void addChildToRootElement(gui::element::AbstractElement *childElement);

    protected:
        gui::element::AbstractElement *elementsTree = nullptr;
        void status(Status s) { _currentStatus = s; }
        void needUpdate(bool needUpdate) { _needUpdate = needUpdate; }

    public:
        virtual ~AbstractManager();
        void setSubRootElement(gui::element::AbstractElement *element);

        void deleteElementsTree();
        void render(bool clear = true);

        Status status() { return _currentStatus; }
        void propagateElementUpdate() { _needUpdate = true; }
        bool needUpdate() { return _needUpdate; }
    };
}

#endif // ABSTRACT_MANAGER_HPP
