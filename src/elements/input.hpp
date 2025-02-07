#ifndef INPUT_HPP
#define INPUT_HPP

#include "label.hpp"

namespace gui {
    namespace element {

        class Input : public Label {
            const std::string placeholder;
            struct {
                int x;
                int y;
            } cursorPos;
            void onFocusGet() override;
            void onFocusLoose() override;

        public:
            Input(const std::string &text = "", const std::string &placeholder = "", gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                  const std::string &identifier = "", TTF_TextEngine *textEngine = nullptr);

            void catchEvent(const SDL_Event &event) override;
        };

    } // namespace element
} // namespace gui

#endif // INPUT_HPP
