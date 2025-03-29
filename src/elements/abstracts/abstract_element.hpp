#ifndef ABSTRACT_ELEMENT_HPP
#define ABSTRACT_ELEMENT_HPP

#include "../../style_nodes/style_nodes_manager.hpp"

#include <string>
#include <vector>

namespace gui {
    namespace element {

        class AbstractElement {
            std::string elementName;
            AbstractElement *_parent = nullptr;
            AbstractElement *_child = nullptr;
            AbstractElement *_next = nullptr;
            gui::elementStyle::manager::StyleNodesManager *elementsStyleManager;
            int _nbChilds = 0;
            bool _updated = false;

        protected:
            gui::elementStyle::StyleNode *elementStyle = nullptr;
            void parent(AbstractElement *parent);
            void updateStyle();

            int nbChilds() { return _nbChilds; }

            void updated() { _updated = true; }

        public:
            bool isUpdated() { return _updated; }
            void updateDone() { _updated = false; }

            /**
             * If no style manager is given, the element can't have style
             */
            AbstractElement(std::string elementName, gui::elementStyle::manager::StyleNodesManager *elementsStyleManager = nullptr,
                            std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
            const std::string &name() const { return elementName; }
            AbstractElement *parent() { return _parent; }
            const AbstractElement *getConstParent() const { return _parent; }
            void addChild(AbstractElement *child);
            AbstractElement *child() { return _child; }
            const AbstractElement *getConstChild() const { return _child; }
            void removeChilds();
            void next(AbstractElement *next) { this->_next = next; }
            AbstractElement *next() { return _next; }
            const AbstractElement *getConstNext() const { return _next; }
            virtual ~AbstractElement();

            /**
             * Override this function to draw self element.
             * Only draw element content (no border / background)
             */
            virtual void renderSelfBeforeChilds() const {};

            /**
             * Override this function to draw self element.
             * Only draw element content (no border / background)
             */
            virtual void renderSelfAfterChilds() const {};

            /**
             * Should call each child with a portion of the element surface
             */
            virtual void renderChilds() const {};

            bool styleManagerAvailable() const { return elementsStyleManager != nullptr; }

            /**
             * true if should propagate to the parent, false else
             */
            void setModifierState(std::string modifierName, bool enabled);
        };

    } // namespace element
} // namespace gui

#endif // ABSTRACT_ELEMENT_HPP