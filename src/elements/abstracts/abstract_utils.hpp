#ifndef ABSTRACT_UTILS_HPP
#define ABSTRACT_UTILS_HPP

#include <stdint.h>

namespace gui::element {
    struct ElementData {};
    struct RenderData {};

    namespace event {
        enum EventType: uint32_t {
            OTHER_EVENTS
        };

        struct Event {
            uint32_t type;
        };

        enum class ElementEvent { REMOVE_CHILDS, ADD_CHILD, CONTENT_CHANGED };
    } // namespace event
} // namespace gui::element
#endif // ABSTRACT_UTILS_HPP