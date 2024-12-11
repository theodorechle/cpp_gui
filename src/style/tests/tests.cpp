#include "tests.hpp"

void Test::lexer(bool equal, const std::string &expr, const Node* expected) {
    std::cerr << "(" << getTestNumber() << ") Test if tokenizing\n'\n" << expr << "\n'\n";
    if (equal) std::cerr << "equals to\n";
    else std::cerr << "differs from\n";
    expected->displayNexts(std::cerr);
    std::cerr << ": ";
    try {
        Node* result = Lexer(expr, settings).getResult();
        Node* n = result;
        while (n != nullptr) {
            if ((expected == nullptr || !(*n ==* expected)) == equal) {
                setTestResult(KO);
                delete result;
                return;
            }
            n = n->getNext();
            expected = expected->getNext();
        }
        if (expected == nullptr) setTestResult(OK);
        else setTestResult(KO);
        delete result;
        displayResult(results.back());
    }
    catch (const std::exception& e) {
        setTestResult(ERROR);
        std::cerr << "Failed with error : " << e.what();
    }
    std::cerr << "\n";
}

void Test::parser(bool equal, Node* expr, const Node* expected) {
    std::cerr << "(" << getTestNumber() << ") Test if parsing\n";
    expr->displayNexts(std::cerr);
    if (equal) std::cerr << "equals to\n";
    else std::cerr << "differs from\n";
    expr->display(std::cerr);
    std::cerr << ": ";
    try {
        Node* result = Parser(expr, settings).getFinalTree();
        if (areSameNodes(result, expected) == equal) setTestResult(OK);
        else setTestResult(KO);
        delete result;
        displayResult(results.back());
    }
    catch (const std::exception& e) {
        setTestResult(ERROR);
        std::cerr << "Failed with error : " << e.what();
    }
    std::cerr << "\n";
}

void Test::lexerAndParser(bool equal, const std::string &expr, const Node* expected) {
    std::cerr << "(" << getTestNumber() << ") Test if tokenizing and parsing\n'\n" << expr << "\n'\n";
    if (equal) std::cerr << "equals to\n";
    else std::cerr << "differs from\n";
    expected->display(std::cerr);
    std::cerr << ": ";
    try {
        Node* tokens = Lexer(expr, settings).getResult();
        Node* result = Parser(tokens, settings).getFinalTree();
        if (areSameNodes(result, expected) == equal) setTestResult(OK);
        else setTestResult(KO);
        delete result;
        delete tokens;
        displayResult(results.back());
    }
    catch (const std::exception& e) {
        setTestResult(ERROR);
        std::cerr << "Failed with error : " << e.what();
    }
    std::cerr << "\n";
}

void Test::invalidExpression(std::string expression) {
    std::cerr << "(" << getTestNumber() << ") Test if tokenizing and parsing\n'\n" << expression << "\n'\n raises a MalformedExpression exception : ";
    Node* tokens = nullptr;
    Node* result = nullptr;
    try {
        tokens = Lexer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        setTestResult(KO);
        displayResult(results.back());
    }
    catch (const MalformedExpression &e) {
        setTestResult(OK);
        displayResult(results.back());
    }
    catch (const std::exception &e) {
        setTestResult(ERROR);
        std::cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    std::cerr << "\n";
}

void Test::unknownToken(std::string expression) {
    std::cerr << "(" << getTestNumber() << ") Test if tokenizing and parsing\n'\n" << expression << "\n'\n raises a UnknownToken exception : ";
    Node* tokens = nullptr;
    Node* result = nullptr;
    try {
        tokens = Lexer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        setTestResult(KO);
        displayResult(results.back());

    }
    catch (const UnknownToken &e) {
        setTestResult(OK);
        displayResult(results.back());

    }
    catch (const std::exception &e) {
        setTestResult(ERROR);
        std::cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    std::cerr << "\n";
}

void Test::missingToken(std::string expression) {
    std::cerr << "(" << getTestNumber() << ") Test if tokenizing and parsing\n'\n" << expression << "\n'\n raises a MissingToken exception : ";
    Node* tokens = nullptr;
    Node* result = nullptr;
    try {
        tokens = Lexer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        setTestResult(KO);
        displayResult(results.back());
    }
    catch (const MissingToken &e) {
        setTestResult(OK);
        displayResult(results.back());
    }
    catch (const std::exception &e) {
        setTestResult(ERROR);
        std::cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    std::cerr << "\n";
}

void Test::unknownValue(std::string expression) {
    std::cerr << "(" << getTestNumber() << ") Test if tokenizing and parsing\n'\n" << expression << "\n'\n raises a UnknownValue exception : ";
    Node* tokens = nullptr;
    Node* result = nullptr;
    try {
        tokens = Lexer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        setTestResult(KO);
        displayResult(results.back());
    }
    catch (const UnknownValue &e) {
        setTestResult(OK);
        displayResult(results.back());
    }
    catch (const std::exception &e) {
        setTestResult(ERROR);
        std::cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    std::cerr << "\n";
}

std::string Test::getFileContent(std::string fileName) {
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Test::displayResult(Result r) const {
    if (r == OK) std::cerr << "OK";
    else if (r == KO) std::cerr << "KO";
    else if (r == ERROR) std::cerr << "ERROR";
    else std::cerr << "Invalid result";
}

void Test::displaySummary() const {
    std::cerr << "Summary:\n";
    size_t index = 0;
    for (std::list<Result>::const_iterator it = results.cbegin(); it != results.cend(); it++) {
        std::cerr << "\ttest " << index << ": ";
        displayResult(*it);
        std::cerr << "\n";
        index++;
    }
    std::cerr << "End of summary\n";
}

int Test::getResultCode() const {
    for (std::list<Result>::const_iterator it = results.cbegin(); it != results.cend(); it++) {
        if (*it != OK) return 1;
    }
    return 0;
}