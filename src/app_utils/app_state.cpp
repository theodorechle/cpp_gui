#include "app_state.hpp"

AppState::AppState(gui::element::manager::AbstractManager *manager, gui::elementStyle::manager::StyleManager *styleManager,
                   TTF_TextEngine *textEngine)
    : _manager{manager}, _styleManager{styleManager} {}

AppState::~AppState() {
    delete _manager;
    delete _styleManager;
    TTF_DestroyRendererTextEngine(_textEngine);
}