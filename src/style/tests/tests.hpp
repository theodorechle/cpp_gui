#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <list>

#include "../node.hpp"
#include "../tokenizer.hpp"
#include "../parser.hpp"
#include "../solver.hpp"

enum Result {
    OK,
    KO,
    ERROR
};

class Test {
    std::list<Result> results;
public:
    size_t getTestNumber() const {return results.size();}
    void setTestResult(Result r) {results.push_back(r);}

    void tokenizer(bool equal, const std::string &expr, const Node *expected, Settings *settings);
    void parser(bool equal, Node *expr, const Node *expected, Settings *settings);
    void tokenizerAndParser(bool equal, const std::string &expr, const Node *expected, Settings *settings);

    std::string boolsEquals(bool equal, bool a, bool b);
    void invalidExpression(std::string expression, Settings *settings);
    void unknownToken(std::string expression, Settings *settings);
    void missingToken(std::string expression, Settings *settings);
    void unknownValue(std::string epxression, Settings *settigns);

    void displayResult(Result r) const;
    void displaySummary() const;

    std::string getFileContent(std::string fileName);
};