#include "manager_actions_service.hpp"
#include "abstract_manager.hpp"

namespace gui {
    namespace element {
        void ManagerActionsService::askRendering() const {
            if (manager != nullptr) manager->askRendering();
        }
        void ManagerActionsService::askRecomputeLayout() const {
            if (manager != nullptr) manager->askRecomputeLayout();
        }
    } // namespace element
} // namespace gui
