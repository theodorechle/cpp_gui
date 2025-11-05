#ifndef ABSTRACT_ELEMENT_HPP
#define ABSTRACT_ELEMENT_HPP

#include "../../../cpp_commons/src/node.hpp"
#include "../../../cpp_style/src/style_nodes/style_nodes_manager.hpp"
#include "abstract_utils.hpp"

#include <string>
#include <vector>
#include <functional>

namespace gui::element {
    namespace manager {
        class AbstractManager;
    }

    class AbstractElement : public commons::Node<AbstractElement> {
        std::string elementName;
        manager::AbstractManager *elementManager = nullptr; 
        style::elementStyle::manager::StyleNodesManager *elementsStyleManager;
        bool _updated = false;

    protected:
        style::elementStyle::StyleNode *style = nullptr;
        void updateStyle();

        void updated() { _updated = true; }

        void sendEventToManager(ElementEvent event);

        std::string debugValue() const override;

    public:
        bool isUpdated() const { return _updated; }
        void updateDone() { _updated = false; }

        /**
         * If no style manager is given, the element can't have style
         */
        AbstractElement(std::string elementName, style::elementStyle::manager::StyleNodesManager *elementsStyleManager = nullptr,
                        std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
        const std::string &name() const { return elementName; }
        void addChild(AbstractElement *newChild);
        // remove pointer to childs, but does not delete them
        void removeChilds();
        void setParent(AbstractElement *parent);

        void manager(manager::AbstractManager *manager);

        virtual ~AbstractElement();

        /**
         * Override this function to draw self element.
         * Only draw element content (no border / background)
         */
        virtual void renderSelfBeforeChildsWrapper() const {};

        /**
         * Override this function to draw self element.
         * Only draw element content (no border / background)
         */
        virtual void renderSelfAfterChildsWrapper() const {};

        /**
         * Should call each child with a portion of the element surface
         */
        virtual void renderChildsWrapper(std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                                         std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const {};

        /**
         * Should call rendering functions, including (if used) previous functions.
         */
        virtual bool render(const ElementData *elementData, std::function<bool(const AbstractElement *, RenderData *)> renderChildCallback,
                            std::function<const ElementData *(const AbstractElement *)> childInfosCallback) const = 0;

        bool styleManagerAvailable() const { return elementsStyleManager != nullptr; }

        /**
         * true if should propagate to the parent, false else
         */
        void setModifierState(std::string modifierName, bool enabled);

        void displayStyle();
    };
} // namespace gui::element

#include "abstract_manager.hpp"

#endif // ABSTRACT_ELEMENT_HPP