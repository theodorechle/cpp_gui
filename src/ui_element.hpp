#ifndef UIELEMENT_HPP
#define UIELEMENT_HPP

#include <list>
#include <string>

class UIElement {
    std::string identifier;
    std::list<std::string> classes;
public:
    UIElement(const std::string &identifier="", std::list<std::string> classes={});
    void refresh();
};

#endif // UIELEMENT_HPP
