#ifndef ABSTRACT_UTILS_HPP
#define ABSTRACT_UTILS_HPP

#include <cstdint>
#include <stdint.h>

namespace gui::element {

    struct ElementData {};
    struct RenderData {};

    namespace event {
        enum EventType : uint32_t { OTHER_EVENTS };

        struct Event {
            uint32_t type;
            Event(uint32_t type) : type{type} {}
            virtual ~Event() {}
            virtual Event *copy() const = 0;
        };

        enum class ElementEvent { REMOVE_CHILDS, ADD_CHILD, CONTENT_CHANGED };
    } // namespace event
} // namespace gui::element
#endif // ABSTRACT_UTILS_HPP
