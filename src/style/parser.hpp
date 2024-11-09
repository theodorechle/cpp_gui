#ifndef PARSER_HPP
#define PARSER_HPP

#include <exception>
#include <unordered_map>
#include <string>
#include "node.hpp"

#include "settings.hpp"


class UnknownToken: public std::exception {
    std::string message;
public:
    UnknownToken(const Node token): message{"Error : Unknown token '" + token.getValue() + " (" + tokenToString(token.getTokenType()) + ")'"} {};
    const char* what() const noexcept override {return message.c_str();}
};

class MissingToken: public std::exception {
    std::string message;
public:
    MissingToken(const std::string &token): message{"Error : Missing token '" + token + "'"} {};
    const char* what() const noexcept override {return message.c_str();}
};

class InvalidExpression: public std::exception {
    std::string message;
public:
    InvalidExpression(const std::string &expression): message{"Error : Invalid expression '" + expression + "'"} {};
    const char *what() const noexcept override {return message.c_str();}
};

class Parser {
    Node *expressionTokens; // maybe change the program to forbid modification
    // only used to avoid recalculing many times the root
    Node *expressionTreeRoot = new Node{Token::NullRoot};
    Node *expressionTree = expressionTreeRoot;
    Settings *settings;

    void getRootOrStopBeforeParenthesis();
    Node *removeParenthesis(Node *t);
    void replaceImplicitTimes(Node *t);

public:
    Parser(Node *expressionTokens, Settings *settings)
    : expressionTokens{expressionTokens}, settings{settings} {
        parse();
    };
    
    /**
     * Transform a chain of trees (no childs) like the one the parser function returns into a tree containing the entire expression
    */
    void parse();
    void parseNumber();
    void parseVariable();
    void parseFunction();
    void parseComma();
    void parseOperator();
    void parseOpeningParenthesis();
    void parseClosingParenthesis();
    void addImplicitMultiplication();
    static bool isNodeNull(Node *node);
    Node *getFinalTree() {return expressionTreeRoot;}
};

#endif // PARSER_HPP