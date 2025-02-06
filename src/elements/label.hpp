#ifndef LABEL_HPP
#define LABEL_HPP

#include "ui_element.hpp"

constexpr const char *FONT_PATH = "src/fonts/";

namespace gui {
    namespace element {
        class Label : public UIElement {
            std::string text;
            TTF_Font *ttfFont = nullptr;
            TTF_Text *ttfText = nullptr;

            void computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight) override;
            void getTextSize(int *width, int *height);
            void renderSelfAfterChilds() override;

        public:
            Label(const std::string &text, gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                  const std::string &identifier = "", TTF_TextEngine *textEngine = nullptr);
                
            ~Label() override;

            SDL_Color textColor() const;
            int fontSize() const;
            std::string fontName() const;
        };

    } // namespace element
} // namespace gui

#endif // LABEL_HPP
