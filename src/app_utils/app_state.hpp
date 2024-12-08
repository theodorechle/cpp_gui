#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include "../managers/abstract_manager.hpp"

class AppState {
    AbstractManager *manager = nullptr;
public:
    AppState(AbstractManager *manager);
    ~AppState();
    AbstractManager *getManager();
};

#endif // APP_STATE_HPP
