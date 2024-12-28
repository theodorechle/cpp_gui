#include "style_tests.hpp"

void StyleTests::lexer(bool equal, const std::string &expr, const Node *expected) {
    startTest();
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
    endTest();
}

void StyleTests::parser(bool equal, Node *expr, const Node *expected) {
    startTest();
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
    endTest();
}

void StyleTests::lexerAndParser(bool equal, const std::string &expr, const Node *expected) {
    startTest();
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
    endTest();
}

void StyleTests::invalidExpression(std::string expression) {
    startTest();
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
    endTest();
}

void StyleTests::unknownToken(std::string expression) {
    startTest();
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
    endTest();
}

void StyleTests::missingToken(std::string expression) {
    startTest();
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
    endTest();
}

void StyleTests::unknownValue(std::string expression) {
    startTest();
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
    endTest();
}
