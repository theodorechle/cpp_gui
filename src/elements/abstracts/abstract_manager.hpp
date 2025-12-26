#ifndef ABSTRACT_MANAGER_HPP
#define ABSTRACT_MANAGER_HPP

#include <algorithm>
#include <list>

#include "../../element_style/style_manager.hpp"
#include "abstract_element.hpp"
#include "abstract_utils.hpp"

namespace gui::element::manager {
    class AbstractManager {
    public:
        enum class Status {
            NOT_STARTED,
            RUNNING,
            ENDED,
            ERROR,
        };

    private:
        Status _currentStatus = Status::NOT_STARTED;
        elementStyle::manager::StyleManager *_styleManager = nullptr;

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

        /**
         * set the modifier's state (enabled, disabled) on leafElement and all its parents
         * if enabled is true, it will also throw the given event on each concerned elements
         */
        void setElementsModifierState(const std::string &modifier, AbstractElement *leafElement, bool enabled, const event::Event *event);

    public:
        virtual ~AbstractManager();
        void setSubRootElement(AbstractElement *element);

        void styleManager(elementStyle::manager::StyleManager *manager) { _styleManager = manager; }

        /**
         * Do actions when an element sends an event (for example, if the element tree changed, it should be re-rendered).
         * This method can be overriden, but if you want to keep the original behavior, don't forget to call this function.
         */
        virtual void elementEvent(event::ElementEvent event, AbstractElement *caller);

        void deleteElementsTree();
        void render(bool clear = true);

        Status status() { return _currentStatus; }
        bool needUpdate() { return !elementsToUpdate.empty(); }
    };
} // namespace gui::element::manager

#endif // ABSTRACT_MANAGER_HPP
