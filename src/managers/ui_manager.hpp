#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "abstract_manager.hpp"

class UIManager: public AbstractManager {
public:
    UIManager();
    void render() override;
};

#endif // UIMANAGER_HPP
