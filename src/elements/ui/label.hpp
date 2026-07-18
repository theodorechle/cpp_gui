#ifndef LABEL_HPP
#define LABEL_HPP

#include "ui_element.hpp"
#include "utils.hpp"

namespace gui::element {
    class Label : public UiElement {
        std::string _text;
        TTF_Font *ttfFont = nullptr;
        TTF_Text *ttfText = nullptr;

        void initBeforeLayoutComputing() override;

        void computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, std::list<std::tuple<int, int>> childsSizes) const override;
        void getTextSize(int *width, int *height) const;
        void renderSelfAfterChilds() const override;

    protected:
        Label(const std::string &elementName, const std::string &text, gui::elementStyle::manager::StyleManager *elementsStyleManager = nullptr,
              std::vector<std::string> classes = {}, const std::string &identifier = "", TTF_TextEngine *textEngine = nullptr);

    public:
        Label(const std::string &text, gui::elementStyle::manager::StyleManager *elementsStyleManager = nullptr,
              std::vector<std::string> classes = {}, const std::string &identifier = "", TTF_TextEngine *textEngine = nullptr);

        ~Label() override;

        SDL_Color textColor() const;
        int fontSize() const;
        std::string fontName() const;
        const std::string &text() { return _text; }

        // methods which creates an internal event

        // append text at end if append is true, else replace the text with the new one
        void setText(const std::string &text, bool append);
        // remove text at end
        void removeText(size_t nbChars);
        void clearText();

    private:
        // actual setters, called by the event handler
        void _setText(const ui::event::TextSetEvent *event);
        void _removeText(size_t nbChars);
        void _clearText();

    public:
        std::string debugValue() const override;
    };
} // namespace gui::element

#endif // LABEL_HPP
