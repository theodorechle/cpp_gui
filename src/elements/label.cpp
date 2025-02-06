#include "label.hpp"

namespace gui {
    namespace element {
        void Label::computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight) {
            TTF_CloseFont(ttfFont);
            ttfFont = TTF_OpenFont(fontName().c_str(), fontSize());
            if (ttfFont == nullptr) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't open font: %s", SDL_GetError());
                return;
            }
            getTextSize(desiredWidth, desiredHeight);
        }

        void Label::getTextSize(int *width, int *height) { TTF_GetStringSizeWrapped(ttfFont, text.c_str(), text.size(), 0, width, height); }

        void Label::renderSelfAfterChilds() {
            SDL_Rect rect;
            SDL_Color color;
            TTF_DestroyText(ttfText);
            SDL_GetRenderClipRect(getRenderer(), &rect);
            if (getTextEngine() == nullptr) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Text engine is not defined.");
                return;
            }

            if (ttfFont == nullptr) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Font is not defined.");
                return;
            }

            ttfText = TTF_CreateText(getTextEngine(), ttfFont, text.c_str(), text.size());
            if (ttfText == nullptr) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't create text: %s", SDL_GetError());
                return;
            }

            color = textColor();
            if (!TTF_SetTextColor(ttfText, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set text color: %s", SDL_GetError());
                return;
            }

            int textWidth, textHeight;
            getTextSize(&textWidth, &textHeight);

            std::string horizontalAlignment = getNameStringFromRule({"horizontal-alignment"}, {"start", "centered", "end"}, "start", true);

            // TODO: add support for reversed languages
            if (horizontalAlignment == "start") {
            }
            else if (horizontalAlignment == "centered") {
                rect.x += (rect.w - textWidth) / 2;
            }
            else {
                rect.x += (rect.w - textWidth);
            }
            std::string verticalAlignment = getNameStringFromRule({"vertical-alignment"}, {"start", "centered", "end"}, "start", true);

            // TODO: add support for reversed languages
            if (verticalAlignment == "start") {
            }
            else if (verticalAlignment == "centered") {
                rect.y += (rect.h - textHeight) / 2;
            }
            else {
                rect.y += (rect.h - textHeight);
            }

            if (!TTF_DrawRendererText(ttfText, rect.x, rect.y)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't render text: %s", SDL_GetError());
            }
        }

        Label::Label(const std::string &text, gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager,
                     std::vector<std::string> *classes, const std::string &identifier, TTF_TextEngine *textEngine)
            : UIElement{"label", elementsStyleManager, classes, identifier, textEngine}, text{text} {}

        Label::~Label() {
            TTF_DestroyText(ttfText);
            TTF_CloseFont(ttfFont);
        }

        SDL_Color Label::textColor() const { return computeColor({"text-color"}, SDL_Color{0, 0, 0, 255}, true); }
        int Label::fontSize() const { return getIntFromRule({"font-size"}, 15, true); }
        std::string Label::fontName() const { return FONT_PATH + getStringFromRule({"font-name"}, "", true); }

    } // namespace element
} // namespace gui
