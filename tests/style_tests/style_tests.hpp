#ifndef STYLE_TESTS_HPP
#define STYLE_TESTS_HPP

#include "../tests.hpp"
#include "../../src/style/style_deserializer.hpp"


class StyleTests : public Tests {

    void tests() override;

public:
    StyleTests() : Tests{"Tests style"} {}
    
    Result testStyleComponentDataList(const StyleComponentDataList *testedData, const StyleComponentDataList *expected);
};

#endif // STYLE_TESTS_HPP
