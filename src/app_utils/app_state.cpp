#include "app_state.hpp"

AppState::AppState(AbstractManager *manager)
: manager{manager} {

}

AppState::~AppState() {
    delete manager;
}

AbstractManager *AppState::getManager() {
    return manager;
}