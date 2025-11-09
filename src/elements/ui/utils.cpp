#include "utils.hpp"

namespace gui::element::ui {
    Uint32 eventCode(Uint32 event) { return guiEventsStartingCode + event - SDL_EVENT_USER; }
} // namespace gui::element::ui