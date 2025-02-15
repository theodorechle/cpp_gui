#include "style_tests_lexer_and_parser.hpp"

namespace styleTestsLexerAndParser {
    test::Result testLexer(bool equal, const std::string &expr, const style::Node *expected) {
        test::Result testResult;
        std::cout << "Test if tokenizing\n'\n" << expr << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->displayNexts(std::cout);
        std::cout << ": ";
        try {
            style::Node *result = style::Lexer(expr).getResult();
            style::Node *n = result;
            while (n != nullptr) {
                if ((expected == nullptr || !(*n == *expected)) == equal) {
                    delete result;
                    return test::Result::FAILURE;
                }
                n = n->getNext();
                expected = expected->getNext();
            }
            if (expected == nullptr) testResult = test::Result::SUCCESS;
            else testResult = test::Result::FAILURE;
            delete result;
        }
        catch (const std::exception &e) {
            testResult = test::Result::ERROR;
            std::cerr << "Failed with error : " << e.what();
        }
        std::cout << "\n";
        return testResult;
    }

    test::Result testParser(bool equal, style::Node *expr, const style::Node *expected) {
        test::Result testResult;
        std::cout << "Test if parsing\n";
        expr->displayNexts(std::cout);
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expr->display(std::cout);
        std::cout << ": ";
        try {
            style::Node *result = style::Parser(expr).getFinalTree();
            if (areSameNodes(result, expected) == equal) testResult = test::Result::SUCCESS;
            else testResult = test::Result::FAILURE;
            delete result;
        }
        catch (const std::exception &e) {
            testResult = test::Result::ERROR;
            std::cerr << "Failed with error : " << e.what();
        }
        std::cout << "\n";
        return testResult;
    }

    test::Result testLexerAndParser(bool equal, const std::string &expr, const style::Node *expected) {
        test::Result testResult;
        std::cout << "Test if tokenizing and parsing\n'\n" << expr << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->display(std::cout);
        std::cout << ": ";
        try {
            style::Node *tokens = style::Lexer(expr).getResult();
            style::Node *result = style::Parser(tokens).getFinalTree();
            if (areSameNodes(result, expected) == equal) testResult = test::Result::SUCCESS;
            else testResult = test::Result::FAILURE;
            delete result;
            delete tokens;
        }
        catch (const std::exception &e) {
            testResult = test::Result::ERROR;
            std::cerr << "Failed with error : " << e.what();
        }
        std::cout << "\n";
        return testResult;
    }

    template <typename T>
    test::Result testLexerAndParserException(const std::string &expression) {
        test::Result testResult;
        std::cout << "Test if tokenizing and parsing\n'\n" << expression << "\n'\n raises an exception : ";
        style::Node *tokens = nullptr;
        style::Node *result = nullptr;
        try {
            tokens = style::Lexer(expression).getResult();
            result = style::Parser(tokens).getFinalTree();
            testResult = test::Result::FAILURE;
        }
        catch (std::exception &exception) {
            if (dynamic_cast<T *>(&exception)) testResult = test::Result::SUCCESS;
            else {
                testResult = test::Result::ERROR;
                std::cerr << "Error : " << exception.what();
            }
        }
        delete tokens;
        delete result;
        std::cout << "\n";
        return testResult;
    }

    std::string getFileContent(std::string fileName) {
        std::ifstream file(fileName);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    test::Result testSingleRule() {
        std::string fileContent;
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-0.txt");
        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "background-color"));
        expected->appendChild(new style::Node(style::Token::Hex, "ff0000"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testTwoStyleBlocks() {
        std::string fileContent;
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-1.txt");

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "label"));
        expected->appendChild(new style::Node(style::Token::Class, "blue"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "text-color"));
        expected->appendChild(new style::Node(style::Token::Hex, "0000ff"));

        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "label"));
        expected->appendChild(new style::Node(style::Token::Class, "blue"));
        expected->appendChild(new style::Node(style::Token::Modifier, "hovered"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "text-color"));
        expected = expected->appendChild(new style::Node(style::Token::Tuple));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "255"));
        expected->appendChild(new style::Node(style::Token::Int, "112"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testNestedModifierBlock() {
        std::string fileContent;
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-2.txt");

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "label"));
        expected->appendChild(new style::Node(style::Token::Class, "blue"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "text-color"));
        expected->appendChild(new style::Node(style::Token::Hex, "0000ff"));
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::Modifier, "hovered"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "text-color"));
        expected = expected->appendChild(new style::Node(style::Token::Tuple));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testNestedElementNameBlock() {
        std::string fileContent;
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-3.txt");

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "label"));
        expected->appendChild(new style::Node(style::Token::Class, "blue"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "text-color"));
        expected->appendChild(new style::Node(style::Token::Hex, "0000ff"));
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "text-color"));
        expected = expected->appendChild(new style::Node(style::Token::Tuple));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testApplyingStyleBlockUsingAnyParentRelation() {
        std::string fileContent;
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-4.txt");

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "label"));
        expected->appendChild(new style::Node(style::Token::Class, "blue"));
        expected->appendChild(new style::Node(style::Token::Modifier, "hovered"));
        expected->appendChild(new style::Node(style::Token::AnyParent));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected->appendChild(new style::Node(style::Token::Class, "red"));
        expected->appendChild(new style::Node(style::Token::Identifier, "root"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "text-color"));
        expected->appendChild(new style::Node(style::Token::Hex, "0000ff"));
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "text-color"));
        expected = expected->appendChild(new style::Node(style::Token::Tuple));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testApplyingStyleBlockUsingAnyChildComponentWithNestedElementName() {
        std::string fileContent;
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-5.txt");

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "label"));
        expected->appendChild(new style::Node(style::Token::Class, "blue"));
        expected->appendChild(new style::Node(style::Token::Modifier, "hovered"));
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected->appendChild(new style::Node(style::Token::Class, "red"));
        expected->appendChild(new style::Node(style::Token::Identifier, "root"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "text-color"));
        expected->appendChild(new style::Node(style::Token::Hex, "0000ff"));
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "text-color"));
        expected = expected->appendChild(new style::Node(style::Token::Tuple));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testMultiLineBlockDeclaration() {
        std::string fileContent;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-6.txt");
        return testLexerAndParserException<style::MalformedExpression>(fileContent);
    }

    test::Result testValuesUnits() {
        std::string fileContent;
        style::Node *rootExpected;
        style::Node *expected;
        test::Result result;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-7.txt");

        rootExpected = new style::Node(style::Token::NullRoot);
        expected = rootExpected->appendChild(new style::Node(style::Token::StyleBlock));
        expected = expected->appendChild(new style::Node(style::Token::BlockDeclaration));
        expected = expected->appendChild(new style::Node(style::Token::Declaration));
        expected->appendChild(new style::Node(style::Token::ElementName, "element"));
        expected = expected->getParent();
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::BlockDefinition));
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "width"));
        expected->appendChild(new style::Node(style::Token::PixelUnit))->appendChild(new style::Node(style::Token::Int, "150"));
        expected = expected->getParent();
        expected = expected->appendChild(new style::Node(style::Token::Assignment));
        expected->appendChild(new style::Node(style::Token::StyleName, "height"));
        expected->appendChild(new style::Node(style::Token::PercentageUnit))->appendChild(new style::Node(style::Token::Int, "40"));

        result = testLexerAndParser(true, fileContent, rootExpected);
        delete rootExpected;
        return result;
    }

    test::Result testNoBlockDeclaration() {
        std::string fileContent;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-8.txt");
        return testLexerAndParserException<style::MalformedExpression>(fileContent);
    }

    test::Result testMultilineCommentNotClosed() {
        std::string fileContent;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-9.txt");
        return testLexerAndParserException<style::UnknownValue>(fileContent);
    }

    void testsLexerAndParser(test::Tests *tests) {
        tests->beginTestBlock("Tests style lexer and parser");

        tests->runTest(testSingleRule, "Single rule");
        tests->runTest(testTwoStyleBlocks, "Two style blocks");
        tests->runTest(testNestedModifierBlock, "Nested modifier block");
        tests->runTest(testNestedElementNameBlock, "Nested element name block");
        tests->runTest(testApplyingStyleBlockUsingAnyParentRelation, "Apply style block using the any parent relation");
        tests->runTest(testApplyingStyleBlockUsingAnyChildComponentWithNestedElementName,
                      "Apply style block to any child component with nested element name");
        tests->runTest(testMultiLineBlockDeclaration, "Multi-line block declaration");
        tests->runTest(testValuesUnits, "Values units");
        tests->runTest(testNoBlockDeclaration, "No block declaration");
        tests->runTest(testMultilineCommentNotClosed, "Multiline comment not closed");

        tests->endTestBlock();
    }

} // namespace styleTestsLexerAndParser
