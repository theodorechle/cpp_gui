#include "style_tests_lexer_and_parser.hpp"

StyleTestsLexerAndParser::StyleTestsLexerAndParser() : Tests{"Tests style lexer and parser"}, settings{new Settings()} { settings->debug = true; }
StyleTestsLexerAndParser::~StyleTestsLexerAndParser() { delete settings; };

void StyleTestsLexerAndParser::tests() {
    std::string fileContent;

    Node *rootExpected;
    Node *expected;

    fileContent = getFileContent(TESTS_FILES_DIR + "/test-0.txt");
    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "background-color"});
    expected->appendChild(new Node{Token::Hex, "ff0000"});

    testLexerAndParser(true, fileContent, rootExpected, "single rule");

    delete rootExpected;
    expected = nullptr;

    fileContent = getFileContent(TESTS_FILES_DIR + "/test-1.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::Hex, "0000ff"});

    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected->appendChild(new Node{Token::Modifier, "hovered"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    testLexerAndParser(true, fileContent, rootExpected, "two style blocks");

    delete rootExpected;
    expected = nullptr;

    fileContent = getFileContent(TESTS_FILES_DIR + "/test-2.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::Hex, "0000ff"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::Modifier, "hovered"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    testLexerAndParser(true, fileContent, rootExpected, "nested modifier block");

    delete rootExpected;
    expected = nullptr;

    fileContent = getFileContent(TESTS_FILES_DIR + "/test-3.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::Hex, "0000ff"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    testLexerAndParser(true, fileContent, rootExpected, "nested element name block");

    delete rootExpected;
    expected = nullptr;

    fileContent = getFileContent(TESTS_FILES_DIR + "/test-4.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected->appendChild(new Node{Token::Modifier, "hovered"});
    expected->appendChild(new Node{Token::AnyParent});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected->appendChild(new Node{Token::Class, "red"});
    expected->appendChild(new Node{Token::Identifier, "root"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::Hex, "0000ff"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    testLexerAndParser(true, fileContent, rootExpected, "apply style block using the any parent relation components");

    delete rootExpected;
    expected = nullptr;

    fileContent = getFileContent(TESTS_FILES_DIR + "/test-5.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected->appendChild(new Node{Token::Modifier, "hovered"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected->appendChild(new Node{Token::Class, "red"});
    expected->appendChild(new Node{Token::Identifier, "root"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::Hex, "0000ff"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    testLexerAndParser(true, fileContent, rootExpected, "apply style block to any child component with nested element name");

    delete rootExpected;
    expected = nullptr;

    fileContent = getFileContent(TESTS_FILES_DIR + "/test-6.txt");
    testLexerAndParserException<MalformedExpression>(fileContent, "raising an error for multi-line block declaration");

    fileContent = getFileContent(TESTS_FILES_DIR + "/test-7.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "width"});
    expected->appendChild(new Node{Token::PixelUnit})->appendChild(new Node{Token::Int, "150"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "height"});
    expected->appendChild(new Node{Token::PercentageUnit})->appendChild(new Node{Token::Int, "40"});

    testLexerAndParser(true, fileContent, rootExpected, "test units for values");

    delete rootExpected;
    expected = nullptr;

    fileContent = getFileContent(TESTS_FILES_DIR + "/test-8.txt");
    testLexerAndParserException<MalformedExpression>(fileContent, "raising an error for no block declaration");
}

void StyleTestsLexerAndParser::testLexer(bool equal, const std::string &expr, const Node *expected, const std::string &testName) {
    Result testResult;
    startTest(testName);
    std::cout << "Test if tokenizing\n'\n" << expr << "\n'\n";
    if (equal) std::cout << "equals to\n";
    else std::cout << "differs from\n";
    expected->displayNexts(std::cout);
    std::cout << ": ";
    try {
        Node *result = Lexer(expr, settings).getResult();
        Node *n = result;
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

void StyleTestsLexerAndParser::testParser(bool equal, Node *expr, const Node *expected, const std::string &testName) {
    Result testResult;
    startTest(testName);
    std::cout << "Test if parsing\n";
    expr->displayNexts(std::cout);
    if (equal) std::cout << "equals to\n";
    else std::cout << "differs from\n";
    expr->display(std::cout);
    std::cout << ": ";
    try {
        Node *result = Parser(expr, settings).getFinalTree();
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

void StyleTestsLexerAndParser::testLexerAndParser(bool equal, const std::string &expr, const Node *expected, const std::string &testName) {
    Result testResult;
    startTest(testName);
    std::cout << "Test if tokenizing and parsing\n'\n" << expr << "\n'\n";
    if (equal) std::cout << "equals to\n";
    else std::cout << "differs from\n";
    expected->display(std::cout);
    std::cout << ": ";
    try {
        Node *tokens = Lexer(expr, settings).getResult();
        Node *result = Parser(tokens, settings).getFinalTree();
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
    std::cout << "Test if tokenizing and parsing\n'\n" << expression << "\n'\n raises a UnknownValue exception : ";
    Node *tokens = nullptr;
    Node *result = nullptr;
    try {
        tokens = Lexer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
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
