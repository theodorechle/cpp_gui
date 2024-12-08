#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include "../managers/abstract_manager.hpp"
#include "../render/abstract_renderer.hpp"

class AppState {
    AbstractManager *manager = nullptr;
    AbstractRenderer *renderer = nullptr;
public:
    AppState(AbstractManager *manager, AbstractRenderer *renderer);
    ~AppState();
    AbstractManager *getManager() const {return manager;}
    AbstractRenderer *getRenderer() const {return renderer;}
};

#endif // APP_STATE_HPP
