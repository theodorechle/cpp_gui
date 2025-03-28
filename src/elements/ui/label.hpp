#ifndef LABEL_HPP
#define LABEL_HPP

#include "ui_element.hpp"

namespace gui {
    namespace element {
        class Label : public UiElement {
            std::string text;
            TTF_Font *ttfFont = nullptr;
            TTF_Text *ttfText = nullptr;

            void initBeforeLayoutComputing() override;

            void computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight) override;
            void getTextSize(int *width, int *height);
            void renderSelfAfterChilds() override;

        protected:
            Label(const std::string &elementName, const std::string &text,
                  gui::elementStyle::manager::StyleNodesManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                  const std::string &identifier = "", TTF_TextEngine *textEngine = nullptr);

        public:
            Label(const std::string &text, gui::elementStyle::manager::StyleNodesManager *elementsStyleManager = nullptr,
                  std::vector<std::string> *classes = nullptr, const std::string &identifier = "", TTF_TextEngine *textEngine = nullptr);

            ~Label() override;

            SDL_Color textColor() const;
            int fontSize() const;
            std::string fontName() const;
            const std::string &getText() { return text; }
            void setText(const std::string &newText);
            void addText(const std::string &toAdd);
            void removeText(int nbChars);
            void clearText();
        };

    } // namespace element
} // namespace gui

#endif // LABEL_HPP
