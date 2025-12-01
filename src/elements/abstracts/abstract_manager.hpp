#ifndef ABSTRACT_MANAGER_HPP
#define ABSTRACT_MANAGER_HPP

#include <algorithm>
#include <list>

#include "abstract_element.hpp"
#include "abstract_utils.hpp"

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
        virtual void renderElements(bool clear = true) const = 0;
        virtual void createRootElement() = 0;
        virtual void update() = 0;
        virtual void addChildToRootElement(AbstractElement *childElement);

    protected:
        AbstractElement *elementsTree = nullptr;
        std::set<AbstractElement *> elementsToUpdate = {};

        void status(Status s) { _currentStatus = s; }

        /**
         * If passed nullptr, clear needed update (no update required)
         */
        void needUpdate(AbstractElement *element);

    public:
        virtual ~AbstractManager();
        void setSubRootElement(AbstractElement *element);

        /**
         * Do actions when an element sends an event (for example, if the element tree changed, it should be re-rendered).
         * This method can be overriden, but if you want to keep the original behavior, don't forget to call this function.
         */
        virtual void elementEvent(ElementEvent event, AbstractElement *caller);

        void deleteElementsTree();
        void render(bool clear = true);

        Status status() { return _currentStatus; }
        bool needUpdate() { return !elementsToUpdate.empty(); }
    };
} // namespace gui::element::manager

#endif // ABSTRACT_MANAGER_HPP
