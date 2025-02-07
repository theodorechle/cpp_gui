#ifndef MANAGER_ACTIONS_SERVICE_HPP
#define MANAGER_ACTIONS_SERVICE_HPP

namespace gui {
    namespace element {
        namespace manager {
            class AbstractManager;
        }

        class ManagerActionsService {
            gui::element::manager::AbstractManager *manager;

        public:
            ManagerActionsService(gui::element::manager::AbstractManager *manager) : manager{manager} {}
            void askRendering() const;
            void askRecomputeLayout() const;
        };

    } // namespace element
} // namespace gui

#endif // MANAGER_ACTIONS_SERVICE_HPP
