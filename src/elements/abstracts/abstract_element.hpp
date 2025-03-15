#ifndef ABSTRACT_ELEMENT_HPP
#define ABSTRACT_ELEMENT_HPP

#include "../../style_nodes/style_nodes_manager.hpp"

#include <string>
#include <vector>

namespace gui {
    namespace element {

        class AbstractElement {
            std::string elementName;
            AbstractElement *parent = nullptr;
            AbstractElement *child = nullptr;
            AbstractElement *next = nullptr;
            gui::elementStyle::manager::StyleNodesManager *elementsStyleManager;
            int nbChilds = 0;

        protected:
            gui::elementStyle::StyleNode *elementStyle = nullptr;
            void setParent(AbstractElement *parent);
            void updateStyle();

            /**
             * Override this function to draw self element.
             * Only draw element content (no border / background)
             */
            virtual void renderSelfBeforeChilds() {};

            /**
             * Override this function to draw self element.
             * Only draw element content (no border / background)
             */
            virtual void renderSelfAfterChilds() {};

            /**
             * Should call each child with a portion of the element surface
             */
            virtual void renderChilds() {};

            int getNbChilds() { return nbChilds; }

        public:
            /**
             * If no style manager is given, the element can't have style
             */
            AbstractElement(std::string elementName, gui::elementStyle::manager::StyleNodesManager *elementsStyleManager = nullptr,
                            std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
            const std::string &getName() const { return elementName; }
            AbstractElement *getParent() { return parent; }
            const AbstractElement *getConstParent() const { return parent; }
            void addChild(AbstractElement *child);
            AbstractElement *getChild() { return child; }
            void removeChilds();
            void setNext(AbstractElement *next) { this->next = next; }
            AbstractElement *getNext() { return next; }
            virtual ~AbstractElement();

            virtual void render() = 0;

            bool styleManagerAvailable() { return elementsStyleManager != nullptr; }

            /**
             * true if should propagate to the parent, false else
             */
            void setModifierState(std::string modifierName, bool enabled);
        };

    } // namespace element
} // namespace gui

#endif // ABSTRACT_ELEMENT_HPP