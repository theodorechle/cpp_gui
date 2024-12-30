#ifndef STYLE_TESTS_HPP
#define STYLE_TESTS_HPP

#include "../tests.hpp"

class StyleTests : public Tests {
    void tests() override;

public:
    StyleTests() : Tests{"Tests style"} {}
};

#endif // STYLE_TESTS_HPP
