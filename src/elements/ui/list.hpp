#ifndef LIST_HPP
#define LIST_HPP

#include "ui_element.hpp"

namespace gui {
    namespace element {

        class List : public UIElement {
            bool vertical = false;
            std::string childsSize = "auto";
            int gap = 0;

            void getMaxDesiredChildsSize(int *desiredWidth, int *desiredHeight, bool vertical);
            void getKeepDesiredChildsSize(int *desiredWidth, int *desiredHeight, bool vertical);
            void getAutoDesiredChildsSize(int *desiredWidth, int *desiredHeight, bool vertical);

            void computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight) override;
            void computeChildsLayout(int x, int y, int availableWidth, int availableHeight) override;

        public:
            List(gui::elementStyle::manager::StyleNodesManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                 const std::string &identifier = "");
        };

    } // namespace element
} // namespace gui

#endif // LIST_HPP
