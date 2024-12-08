#include "abstract_manager.hpp"

AbstractElement::AbstractElement(AbstractManager *manager)
: manager{manager} {
    manager->addElement(this);
}

void AbstractManager::addElement(AbstractElement* element) {
    elementsList.push_back(element);
}

void AbstractManager::removeElement(AbstractElement* element, bool delete_if_found) {
    if (delete_if_found && std::find(elementsList.cbegin(), elementsList.cend(), element) != elementsList.cend()) {
        delete element;
    }
    elementsList.remove(element);
}

void AbstractManager::removeAllElements(bool delete_if_found) {
    if (delete_if_found) {
        for (AbstractElement* element : elementsList) {
            delete element;
        }
    }
    elementsList.clear();
}
