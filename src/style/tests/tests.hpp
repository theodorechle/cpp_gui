#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <list>

#include "../node.hpp"
#include "../lexer.hpp"
#include "../parser.hpp"
#include "../solver.hpp"

enum Result {
    OK,
    KO,
    ERROR
};

class Test {
    std::list<Result> results;
    Settings* settings;
public:
    Test(Settings* settings): settings{settings} {}
    size_t getTestNumber() const {return results.size();}
    void setTestResult(Result r) {results.push_back(r);}

    void lexer(bool equal, const std::string &expr, const Node* expected);
    void parser(bool equal, Node* expr, const Node* expected);
    void lexerAndParser(bool equal, const std::string &expr, const Node* expected);

    std::string boolsEquals(bool equal, bool a, bool b);
    void invalidExpression(std::string expression);
    void unknownToken(std::string expression);
    void missingToken(std::string expression);
    void unknownValue(std::string epxression);

    void displayResult(Result r) const;
    void displaySummary() const;

    std::string getFileContent(std::string fileName);
};