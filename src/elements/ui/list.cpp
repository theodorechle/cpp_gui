#include "list.hpp"

namespace gui {
    namespace element {
        void List::getMaxDesiredChildsSize(int *desiredWidth, int *desiredHeight, bool vertical) {
            int nbChilds = 0;
            (*desiredWidth) = 0;
            (*desiredHeight) = 0;
            int childDesiredWidth = 0;
            int childDesiredHeight = 0;
            UiElement *child = getChild();
            while (child != nullptr) {
                child->setMarginsActive(false);
                child->computeDesiredLayout(&childDesiredWidth, &childDesiredHeight);
                (*desiredWidth) = std::max(childDesiredWidth, *desiredWidth);
                (*desiredHeight) = std::max(childDesiredHeight, *desiredHeight);
                nbChilds++;
                child = child->getNext();
            }
            if (vertical) (*desiredHeight) *= nbChilds;
            else (*desiredWidth) *= nbChilds;
        }

        void List::getKeepDesiredChildsSize(int *desiredWidth, int *desiredHeight, bool vertical) {
            (*desiredWidth) = 0;
            (*desiredHeight) = 0;
            int childDesiredWidth = 0;
            int childDesiredHeight = 0;
            UiElement *child = getChild();
            while (child != nullptr) {
                child->setMarginsActive(false);
                child->computeDesiredLayout(&childDesiredWidth, &childDesiredHeight);
                if (vertical) {
                    (*desiredWidth) = std::max(*desiredWidth, childDesiredWidth);
                    (*desiredHeight) += childDesiredHeight;
                }
                else {
                    (*desiredWidth) += childDesiredWidth;
                    (*desiredHeight) = std::max(*desiredHeight, childDesiredHeight);
                }
                child = child->getNext();
            }
        }

        void List::getAutoDesiredChildsSize(int *desiredWidth, int *desiredHeight, bool vertical) {
            (*desiredWidth) = 0;
            (*desiredHeight) = 0;
            int childDesiredWidth = 0;
            int childDesiredHeight = 0;
            UiElement *child = getChild();
            while (child != nullptr) {
                child->setMarginsActive(false);
                child->computeDesiredLayout(&childDesiredWidth, &childDesiredHeight);
                if (vertical) {
                    (*desiredWidth) = std::max(*desiredWidth, childDesiredWidth);
                }
                else {
                    (*desiredHeight) = std::max(*desiredHeight, childDesiredHeight);
                }
                child = child->getNext();
            }
        }

        void List::computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight) {
            vertical = getBoolFromRule({"vertical"});
            childsSize = getNameStringFromRule("childs-size", {"biggest", "auto", "keep"}, "auto");
            if (childsSize == "biggest") getMaxDesiredChildsSize(desiredWidth, desiredHeight, vertical);
            else if (childsSize == "keep") getKeepDesiredChildsSize(desiredWidth, desiredHeight, vertical);
            else if (childsSize == "auto") getAutoDesiredChildsSize(desiredWidth, desiredHeight, vertical);
            if (vertical) {
                gap = computeSize({"gap"}, 0, false, (getParent() == nullptr) ? 0 : getParent()->getHeight()) * (nbChilds() - 1);
                (*desiredHeight) += gap;
            }
            else {
                gap = computeSize({"gap"}, 0, false, (getParent() == nullptr) ? 0 : getParent()->getWidth()) * (nbChilds() - 1);
                (*desiredWidth) += gap;
            }
        }

        void List::computeChildsLayout(int x, int y, int availableWidth, int availableHeight) {
            int childWidth = 0;
            int childHeight = 0;
            int tmpChildWidth = 0;
            int tmpChildHeight = 0;

            if (vertical) {
                childWidth = availableWidth;
            }
            else {
                childHeight = availableHeight;
            }

            if (childsSize == "biggest" || childsSize == "auto") {
                if (vertical) {
                    childHeight = availableHeight / nbChilds();
                }
                else {
                    childWidth = availableWidth / nbChilds();
                }
            }
            UiElement *child = getChild();
            while (child != nullptr) {
                if (childsSize == "keep") {
                    child->getDesiredSize(&tmpChildWidth, &tmpChildHeight);
                    if (vertical) childHeight = tmpChildHeight;
                    else childWidth = tmpChildWidth;
                }
                child->computeLayout(x, y, std::min(childWidth, availableWidth), std::min(childHeight, availableHeight));
                child = child->getNext();
                if (vertical) y += childHeight + gap;
                else x += childWidth + gap;
            }
        }

        List::List(gui::elementStyle::manager::StyleNodesManager *elementsStyleManager, std::vector<std::string> *classes,
                   const std::string &identifier)
            : UiElement{"list", elementsStyleManager, classes, identifier} {}
    } // namespace element
} // namespace gui
