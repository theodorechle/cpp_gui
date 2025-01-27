#ifndef TABLE_HPP
#define TABLE_HPP

#include "ui_element.hpp"

namespace gui {
    namespace element {

        class Table : public UIElement {
        public:
            Table(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                  const std::string &identifier = "");
        };

    } // namespace element
} // namespace gui

#endif // TABLE_HPP
