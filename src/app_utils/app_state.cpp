#include "app_state.hpp"

AppState::AppState(AbstractManager *manager, AbstractRenderer *renderer)
: manager{manager}, renderer{renderer} {

}

AppState::~AppState() {
    delete manager;
}