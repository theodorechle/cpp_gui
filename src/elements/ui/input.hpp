#ifndef INPUT_HPP
#define INPUT_HPP

#include "label.hpp"

namespace gui::element {
    class Input : public Label {
        const std::string placeholder;
        struct {
            int x;
            int y;
        } cursorPos;

    public:
        Input(const std::string &text = "", const std::string &placeholder = "",
              gui::elementStyle::manager::StyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
              const std::string &identifier = "", TTF_TextEngine *textEngine = nullptr);
        
        void focusGained(const event::Event *event);

        void focusLost(const event::Event *event);

        void textInput(const ui::event::TextEvent *event);

        void keyDown(const ui::event::KeyEvent *event);
    };
} // namespace gui::element

#endif // INPUT_HPP
