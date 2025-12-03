#ifndef ABSTRACT_ELEMENT_HPP
#define ABSTRACT_ELEMENT_HPP

#include "../../../cpp_commons/src/node.hpp"
#include "../../../cpp_style/src/style_component.hpp"
#include "../../element_style/element_style.hpp"
#include "abstract_utils.hpp"

#include <functional>
#include <string>
#include <vector>

namespace gui::elementStyle::manager {
    class StyleManager;
}
namespace gui::element {
    namespace manager {
        class AbstractManager;
    }

    class AbstractElement : public commons::Node<AbstractElement> {
        std::string elementName;
        manager::AbstractManager *_manager = nullptr;
        elementStyle::manager::StyleManager *elementsStyleManager;

    protected:
        elementStyle::ElementStyle _style;

        void updateStyle();

        void sendEventToManager(ElementEvent event);

        std::string debugValue() const override;

    public:
        /**
         * If no style manager is given, the element can't have style
         */
        AbstractElement(std::string elementName, elementStyle::manager::StyleManager *elementsStyleManager = nullptr,
                        std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
        const std::string &name() const { return elementName; }
        void addChild(AbstractElement *newChild);
        // remove pointer to childs, but does not delete them
        void removeChilds();
        void setParent(AbstractElement *parent);

        void manager(manager::AbstractManager *manager);
        manager::AbstractManager *manager() { return _manager; }

        elementStyle::ElementStyle *style() { return &_style; }

        virtual ~AbstractElement() {};

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

        void displayStyle();
    };
} // namespace gui::element

#include "../../element_style/style_manager.hpp"
#include "abstract_manager.hpp"

#endif // ABSTRACT_ELEMENT_HPP