#ifndef LABEL_HPP
#define LABEL_HPP

#include "ui_element.hpp"
#include <filesystem>

namespace gui::element {
    class Label : public UiElement {
        std::string _text;
        TTF_Font *ttfFont = nullptr;
        TTF_Text *ttfText = nullptr;

        void initBeforeLayoutComputing() override;

        void computeInnerLayout(int *width, int *height) const override;
        void getTextSize(int *width, int *height) const;
        void renderSelfAfterChilds() const override;

    protected:
        Label(const std::string &elementName, const std::string &text, gui::elementStyle::manager::StyleManager *elementsStyleManager = nullptr,
              std::vector<std::string> *classes = nullptr, const std::string &identifier = "", TTF_TextEngine *textEngine = nullptr);

    public:
        Label(const std::string &text, gui::elementStyle::manager::StyleManager *elementsStyleManager = nullptr,
              std::vector<std::string> *classes = nullptr, const std::string &identifier = "", TTF_TextEngine *textEngine = nullptr);

        ~Label() override;

        SDL_Color textColor() const;
        int fontSize() const;
        std::string fontName() const;
        const std::string &text() { return _text; }
        void text(const std::string &newText) { _text = newText; }
        void addText(const std::string &toAdd);
        void removeText(int nbChars);
        void clearText();
    };
} // namespace gui::element

#endif // LABEL_HPP
