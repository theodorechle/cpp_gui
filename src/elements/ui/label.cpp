#include "label.hpp"

namespace gui {
    namespace element {
        void Label::initBeforeLayoutComputing() {
            TTF_CloseFont(ttfFont);
            ttfFont = TTF_OpenFont(fontName().c_str(), fontSize());
            if (ttfFont == nullptr) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Label::initBeforeLayoutComputing: Can't open font: %s", SDL_GetError());
                return;
            }

            TTF_DestroyText(ttfText);
            ttfText = TTF_CreateText(getTextEngine(), ttfFont, text.c_str(), text.size());
            if (ttfText == nullptr) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Label::initBeforeLayoutComputing: Can't create text: %s", SDL_GetError());
                return;
            }

            int style = TTF_STYLE_NORMAL;
            std::string fontWeight = getNameStringFromRule("font-weight", {"normal", "bold"}, "normal", true);
            if (fontWeight == "bold") style |= TTF_STYLE_BOLD;
            if (getBoolFromRule({"font-italic"}, false, true)) style |= TTF_STYLE_ITALIC;
            if (getBoolFromRule({"font-underline"}, false, true)) style |= TTF_STYLE_UNDERLINE;
            if (getBoolFromRule({"font-strike-through"}, false, true)) style |= TTF_STYLE_STRIKETHROUGH;
            TTF_SetFontStyle(ttfFont, style);
        }

        void Label::computeInnerLayout(int *width, int *height) const { getTextSize(width, height); }

        void Label::getTextSize(int *width, int *height) const {
            if (ttfFont) {
                std::string wrapping = getNameStringFromRule("text-wrap", {"wrapped", "line-break"}, "line-break", true);
                if (wrapping == "wrapped") {
                    int wrapWidth = this->width();
                    if (wrapWidth != 0) {
                        TTF_GetStringSizeWrapped(ttfFont, text.c_str(), text.size(), wrapWidth, width, height);
                        return;
                    }
                }
                else if (wrapping == "line-break") {
                    TTF_GetStringSizeWrapped(ttfFont, text.c_str(), text.size(), 0, width, height);
                    return;
                }
            }
            (*width) = 0;
            (*height) = 0;
        }

        void Label::renderSelfAfterChilds() const {
            SDL_Rect rect;
            SDL_Color color;
            SDL_GetRenderClipRect(getRenderer(), &rect);
            if (getTextEngine() == nullptr) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Label::renderSelfAfterChilds: Text engine is not defined.");
                return;
            }

            if (ttfText == nullptr) return;

            std::string wrapping = getNameStringFromRule("text-wrap", {"wrapped", "line-break"}, "line-break", true);
            if (wrapping == "wrapped") {
                int wrapWidth = rect.w;
                if (!TTF_SetTextWrapWidth(ttfText, wrapWidth)) {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Label::renderSelfAfterChilds: Can't set text wrap width: %s", SDL_GetError());
                }
            }
            else if (wrapping == "line-break") {
                if (!TTF_SetTextWrapWidth(ttfText, 0)) {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Label::renderSelfAfterChilds: Can't set text wrap width: %s", SDL_GetError());
                }
            }

            color = textColor();
            if (!TTF_SetTextColor(ttfText, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Label::renderSelfAfterChilds: Can't set text color: %s", SDL_GetError());
                return;
            }

            int textWidth, textHeight;
            getTextSize(&textWidth, &textHeight);

            std::string horizontalAlignment = getNameStringFromRule("horizontal-alignment", {"start", "centered", "end"}, "start", true);

            // TODO: add support for reversed languages
            if (horizontalAlignment == "start") {
            }
            else if (horizontalAlignment == "centered") {
                rect.x += (rect.w - textWidth) / 2;
            }
            else {
                rect.x += (rect.w - textWidth);
            }
            std::string verticalAlignment = getNameStringFromRule("vertical-alignment", {"start", "centered", "end"}, "start", true);

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
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Label::renderSelfAfterChilds: Can't render text: %s", SDL_GetError());
            }
        }

        Label::Label(const std::string &elementName, const std::string &text, gui::elementStyle::manager::StyleNodesManager *elementsStyleManager,
                     std::vector<std::string> *classes, const std::string &identifier, TTF_TextEngine *textEngine)
            : UiElement{elementName, elementsStyleManager, classes, identifier, textEngine}, text{text} {}

        Label::Label(const std::string &text, gui::elementStyle::manager::StyleNodesManager *elementsStyleManager, std::vector<std::string> *classes,
                     const std::string &identifier, TTF_TextEngine *textEngine)
            : UiElement{"label", elementsStyleManager, classes, identifier, textEngine}, text{text} {}

        Label::~Label() {
            TTF_DestroyText(ttfText);
            TTF_CloseFont(ttfFont);
        }

        SDL_Color Label::textColor() const { return computeColor({"text-color"}, SDL_Color{0, 0, 0, 255}, true); }
        int Label::fontSize() const { return getIntFromRule({"font-size"}, 15, true); }
        std::string Label::fontName() const { return style->getFontsPath() + getStringFromRule({"font-name"}, "", true); }

        void Label::setText(const std::string &newText) {
            text = newText;
            updated();
        }

        void Label::addText(const std::string &toAdd) {
            text.append(toAdd);
            updated();
        }

        void Label::removeText(int nbChars) {
            if (text.empty()) return;
            text.resize(text.size() - nbChars);
            updated();
        }

        void Label::clearText() {
            text.clear();
            updated();
        }

    } // namespace element
} // namespace gui
