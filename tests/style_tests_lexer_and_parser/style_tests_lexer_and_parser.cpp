#include "style_tests_lexer_and_parser.hpp"

namespace test {

    StyleTestsLexerAndParser::StyleTestsLexerAndParser() : Tests{"Tests style lexer and parser"}, settings{new style::Settings()} { settings->debug = true; }
    StyleTestsLexerAndParser::~StyleTestsLexerAndParser() { delete settings; };

    void StyleTestsLexerAndParser::tests() {
        std::string fileContent;

        style::Node *rootExpected;
        style::Node *expected;

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

        testLexerAndParser(true, fileContent, rootExpected, "Single rule");

        delete rootExpected;
        expected = nullptr;

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
        expected->appendChild(new style::Node(style::Token::Int, "150"));
        expected->appendChild(new style::Node(style::Token::Int, "150"));

        testLexerAndParser(true, fileContent, rootExpected, "Two style blocks");

        delete rootExpected;
        expected = nullptr;

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

        testLexerAndParser(true, fileContent, rootExpected, "Nested modifier block");

        delete rootExpected;
        expected = nullptr;

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

        testLexerAndParser(true, fileContent, rootExpected, "Nested element name block");

        delete rootExpected;
        expected = nullptr;

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

        testLexerAndParser(true, fileContent, rootExpected, "Apply style block using the any parent relation components");

        delete rootExpected;
        expected = nullptr;

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

        testLexerAndParser(true, fileContent, rootExpected, "Apply style block to any child component with nested element name");

        delete rootExpected;
        expected = nullptr;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-6.txt");
        testLexerAndParserException<style::MalformedExpression>(fileContent, "Multi-line block declaration");

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

        testLexerAndParser(true, fileContent, rootExpected, "Values units");

        delete rootExpected;
        expected = nullptr;

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-8.txt");
        testLexerAndParserException<style::MalformedExpression>(fileContent, "No block declaration");

        fileContent = getFileContent(TESTS_FILES_DIR + "/test-9.txt");
        testLexerAndParserException<style::LexerError>(fileContent, "Multiline comment not closed"); // FIXME: memory leak
    }

    void StyleTestsLexerAndParser::testLexer(bool equal, const std::string &expr, const style::Node *expected, const std::string &testName) {
        Result testResult;
        startTest(testName);
        std::cout << "Test if tokenizing\n'\n" << expr << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->displayNexts(std::cout);
        std::cout << ": ";
        try {
            style::Node *result = style::Lexer(expr, settings).getResult();
            style::Node *n = result;
            while (n != nullptr) {
                if ((expected == nullptr || !(*n == *expected)) == equal) {
                    testResult = Result::KO;
                    delete result;
                    return;
                }
                n = n->getNext();
                expected = expected->getNext();
            }
            if (expected == nullptr) testResult = Result::OK;
            else testResult = Result::KO;
            delete result;
        }
        catch (const std::exception &e) {
            testResult = Result::ERROR;
            std::cerr << "Failed with error : " << e.what();
        }
        std::cout << "\n";
        endTest(testResult);
    }

    void StyleTestsLexerAndParser::testParser(bool equal, style::Node *expr, const style::Node *expected, const std::string &testName) {
        Result testResult;
        startTest(testName);
        std::cout << "Test if parsing\n";
        expr->displayNexts(std::cout);
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expr->display(std::cout);
        std::cout << ": ";
        try {
            style::Node *result = style::Parser(expr, settings).getFinalTree();
            if (areSameNodes(result, expected) == equal) testResult = Result::OK;
            else testResult = Result::KO;
            delete result;
        }
        catch (const std::exception &e) {
            testResult = Result::ERROR;
            std::cerr << "Failed with error : " << e.what();
        }
        std::cout << "\n";
        endTest(testResult);
    }

    void StyleTestsLexerAndParser::testLexerAndParser(bool equal, const std::string &expr, const style::Node *expected, const std::string &testName) {
        Result testResult;
        startTest(testName);
        std::cout << "Test if tokenizing and parsing\n'\n" << expr << "\n'\n";
        if (equal) std::cout << "equals to\n";
        else std::cout << "differs from\n";
        expected->display(std::cout);
        std::cout << ": ";
        try {
            style::Node *tokens = style::Lexer(expr, settings).getResult();
            style::Node *result = style::Parser(tokens, settings).getFinalTree();
            if (areSameNodes(result, expected) == equal) testResult = Result::OK;
            else testResult = Result::KO;
            delete result;
            delete tokens;
        }
        catch (const std::exception &e) {
            testResult = Result::ERROR;
            std::cerr << "Failed with error : " << e.what();
        }
        std::cout << "\n";
        endTest(testResult);
    }

    template <typename T> void StyleTestsLexerAndParser::testLexerAndParserException(const std::string &expression, const std::string &testName) {
        Result testResult;
        startTest(testName);
        std::cout << "Test if tokenizing and parsing\n'\n" << expression << "\n'\n raises an exception : ";
        style::Node *tokens = nullptr;
        style::Node *result = nullptr;
        try {
            tokens = style::Lexer(expression, settings).getResult();
            result = style::Parser(tokens, settings).getFinalTree();
            testResult = Result::KO;
        }
        catch (std::exception &exception) {
            if (dynamic_cast<T *>(&exception)) testResult = Result::OK;
            else {
                testResult = Result::ERROR;
                std::cerr << "Error : " << exception.what();
            }
        }
        delete tokens;
        delete result;
        std::cout << "\n";
        endTest(testResult);
    }

} // namespace test
