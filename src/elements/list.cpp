#include "list.hpp"

namespace gui {
    namespace element {
        void List::getTotalDesiredChildsSize(int *desiredWidth, int *desiredHeight, bool vertical) {
            (*desiredWidth) = 0;
            (*desiredHeight) = 0;
            int childDesiredWidth = 0;
            int childDesiredHeight = 0;
            UIElement *child = getChild();
            while (child != nullptr) {
                child->setMarginsActive(false);
                child->computeDesiredLayout(&childDesiredWidth, &childDesiredHeight);
                if (!child->isSizeParentRelative()) {
                    if (vertical) {
                        (*desiredWidth) = std::max(*desiredWidth, childDesiredWidth);
                        (*desiredHeight) += childDesiredHeight;
                    }
                    else {
                        (*desiredWidth) += childDesiredWidth;
                        (*desiredHeight) = std::max(*desiredHeight, childDesiredHeight);
                    }
                }
                child = child->getNext();
            }
        }

        void List::getMaxDesiredChildsSize(int *desiredWidth, int *desiredHeight, bool vertical) {
            int nbChilds = 0;
            (*desiredWidth) = 0;
            (*desiredHeight) = 0;
            int childDesiredWidth = 0;
            int childDesiredHeight = 0;
            UIElement *child = getChild();
            while (child != nullptr) {
                child->setMarginsActive(false);
                child->computeDesiredLayout(&childDesiredWidth, &childDesiredHeight);
                if (!child->isSizeParentRelative()) {
                    (*desiredWidth) = std::max(childDesiredWidth, *desiredWidth);
                    (*desiredHeight) = std::max(childDesiredHeight, *desiredHeight);
                    nbChilds++;
                }
                child = child->getNext();
            }
            if (vertical) (*desiredHeight) *= nbChilds;
            else (*desiredWidth) *= nbChilds;
        }

        void List::computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight) {
            vertical = getBoolFromRule({"vertical"});
            childsSize = getNameStringFromRule({"childs-size"}, {"biggest", "auto"}, "auto");
            if (childsSize == "biggest") getMaxDesiredChildsSize(desiredWidth, desiredHeight, vertical);
            else getTotalDesiredChildsSize(desiredWidth, desiredHeight, vertical);
            if (vertical)
                (*desiredHeight) += computeSize({"gap"}, 0, false, (getParent() == nullptr) ? 0 : getParent()->getHeight()) * (getNbChilds() - 1);
            else (*desiredWidth) += computeSize({"gap"}, 0, false, (getParent() == nullptr) ? 0 : getParent()->getHeight()) * (getNbChilds() - 1);
        }

        void List::computeChildsLayout(int x, int y, int availableWidth, int availableHeight) {
            int childWidth = 0;
            int childHeight = 0;
            int gap;
            if (vertical) gap = computeSize({"gap"}, 0, false, (getParent() == nullptr) ? 0 : getParent()->getHeight());
            else gap = computeSize({"gap"}, 0, false, (getParent() == nullptr) ? 0 : getParent()->getHeight());
            if (childsSize == "biggest") {
                if (vertical) {
                    childWidth = availableWidth;
                    childHeight = availableHeight / getNbChilds();
                }
                else {
                    childWidth = availableWidth / getNbChilds();
                    childHeight = availableHeight;
                }
            }
            UIElement *child = getChild();
            while (child != nullptr) {
                if (childsSize == "auto") child->getDesiredSize(&childWidth, &childHeight);
                child->computeLayout(x, y, std::min(childWidth, availableWidth), std::min(childHeight, availableHeight));
                child = child->getNext();
                if (vertical) y += childHeight + gap;
                else x += childWidth + gap;
            }
        }

        List::List(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                   const std::string &identifier)
            : UIElement{"list", elementsStyleManager, classes, identifier} {}
    } // namespace element
} // namespace gui
