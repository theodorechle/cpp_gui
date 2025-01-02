#include "style_tests_lexer_and_parser.hpp"

StyleTestsLexerAndParser::StyleTestsLexerAndParser() : Tests{"Tests style lexer and parser"}, settings{new Settings()} {
    settings->debug = true;
}
StyleTestsLexerAndParser::~StyleTestsLexerAndParser() {
    delete settings;
};

void StyleTestsLexerAndParser::tests() {
    std::string fileContent;

    Node *rootExpected;
    Node *expected;

    startTest("single rule");
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
    expected->appendChild(new Node{Token::String, "#ff0000"});

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;
    endTest();

    startTest("two style blocks");
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
    expected->appendChild(new Node{Token::String, "#0000ff"});

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

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;
    endTest();

    startTest("nested modifier block");
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
    expected->appendChild(new Node{Token::String, "#0000ff"});
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

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;
    endTest();

    startTest("nested element name block");
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
    expected->appendChild(new Node{Token::String, "#0000ff"});
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

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;
    endTest();

    startTest("apply style block using the any parent relation components");
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
    expected->appendChild(new Node{Token::String, "#0000ff"});
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

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;
    endTest();

    startTest("apply style block to any child component with nested element name");
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
    expected->appendChild(new Node{Token::String, "#0000ff"});
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

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;
    endTest();

    startTest("raising an error for multi-line block declaration");
    fileContent = getFileContent(TESTS_FILES_DIR + "/test-6.txt");
    invalidExpression(fileContent);
    endTest();

    startTest("test units for values");
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

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;
    endTest();

    startTest("raising an error for no block declaration");
    fileContent = getFileContent(TESTS_FILES_DIR + "/test-8.txt");
    invalidExpression(fileContent);
    endTest();
}

void StyleTestsLexerAndParser::lexer(bool equal, const std::string &expr, const Node *expected) {
    std::cout << "Test if tokenizing\n'\n"
              << expr << "\n'\n";
    if (equal) std::cout << "equals to\n";
    else std::cout << "differs from\n";
    expected->displayNexts(std::cout);
    std::cout << ": ";
    try {
        Node *result = Lexer(expr, settings).getResult();
        Node *n = result;
        while (n != nullptr) {
            if ((expected == nullptr || !(*n == *expected)) == equal) {
                setTestResult(Result::KO);
                delete result;
                return;
            }
            n = n->getNext();
            expected = expected->getNext();
        }
        if (expected == nullptr) setTestResult(Result::OK);
        else setTestResult(Result::KO);
        delete result;
    }
    catch (const std::exception &e) {
        setTestResult(Result::ERROR);
        std::cerr << "Failed with error : " << e.what();
    }
    std::cout << "\n";
}

void StyleTestsLexerAndParser::parser(bool equal, Node *expr, const Node *expected) {
    std::cout << "Test if parsing\n";
    expr->displayNexts(std::cout);
    if (equal) std::cout << "equals to\n";
    else std::cout << "differs from\n";
    expr->display(std::cout);
    std::cout << ": ";
    try {
        Node *result = Parser(expr, settings).getFinalTree();
        if (areSameNodes(result, expected) == equal) setTestResult(Result::OK);
        else setTestResult(Result::KO);
        delete result;
    }
    catch (const std::exception &e) {
        setTestResult(Result::ERROR);
        std::cerr << "Failed with error : " << e.what();
    }
    std::cout << "\n";
}

void StyleTestsLexerAndParser::lexerAndParser(bool equal, const std::string &expr, const Node *expected) {
    std::cout << "Test if tokenizing and parsing\n'\n"
              << expr << "\n'\n";
    if (equal) std::cout << "equals to\n";
    else std::cout << "differs from\n";
    expected->display(std::cout);
    std::cout << ": ";
    try {
        Node *tokens = Lexer(expr, settings).getResult();
        Node *result = Parser(tokens, settings).getFinalTree();
        if (areSameNodes(result, expected) == equal) setTestResult(Result::OK);
        else setTestResult(Result::KO);
        delete result;
        delete tokens;
    }
    catch (const std::exception &e) {
        setTestResult(Result::ERROR);
        std::cerr << "Failed with error : " << e.what();
    }
    std::cout << "\n";
}

void StyleTestsLexerAndParser::invalidExpression(std::string expression) {
    std::cout << "Test if tokenizing and parsing\n'\n"
              << expression << "\n'\n raises a MalformedExpression exception : ";
    Node *tokens = nullptr;
    Node *result = nullptr;
    try {
        tokens = Lexer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        setTestResult(Result::KO);
    }
    catch (const MalformedExpression &e) {
        setTestResult(Result::OK);
    }
    catch (const std::exception &e) {
        setTestResult(Result::ERROR);
        std::cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    std::cout << "\n";
}

void StyleTestsLexerAndParser::unknownToken(std::string expression) {
    std::cout << "Test if tokenizing and parsing\n'\n"
              << expression << "\n'\n raises a UnknownToken exception : ";
    Node *tokens = nullptr;
    Node *result = nullptr;
    try {
        tokens = Lexer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        setTestResult(Result::KO);
    }
    catch (const UnknownToken &e) {
        setTestResult(Result::OK);
    }
    catch (const std::exception &e) {
        setTestResult(Result::ERROR);
        std::cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    std::cout << "\n";
}

void StyleTestsLexerAndParser::missingToken(std::string expression) {
    std::cout << "Test if tokenizing and parsing\n'\n"
              << expression << "\n'\n raises a MissingToken exception : ";
    Node *tokens = nullptr;
    Node *result = nullptr;
    try {
        tokens = Lexer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        setTestResult(Result::KO);
    }
    catch (const MissingToken &e) {
        setTestResult(Result::OK);
    }
    catch (const std::exception &e) {
        setTestResult(Result::ERROR);
        std::cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    std::cout << "\n";
}

void StyleTestsLexerAndParser::unknownValue(std::string expression) {
    std::cout << "Test if tokenizing and parsing\n'\n"
              << expression << "\n'\n raises a UnknownValue exception : ";
    Node *tokens = nullptr;
    Node *result = nullptr;
    try {
        tokens = Lexer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        setTestResult(Result::KO);
    }
    catch (const UnknownValue &e) {
        setTestResult(Result::OK);
    }
    catch (const std::exception &e) {
        setTestResult(Result::ERROR);
        std::cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    std::cout << "\n";
}
