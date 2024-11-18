#ifndef PARSER_HPP
#define PARSER_HPP

#include <exception>
#include <unordered_map>
#include <string>
#include "node.hpp"

#include "settings.hpp"

class ParserError: public std::exception {
    std::string message;
public:
    ParserError(const std::string& message): message{message} {};
    const char* what() const noexcept override {return message.c_str();}
};

class UnknownToken: public ParserError {
public:
    UnknownToken(const Node token): ParserError{"Unknown token: \"" + token.getValue() + " (" + tokenToString(token.getTokenType()) + ")\""} {};
};

class MissingToken: public ParserError {
public:
    MissingToken(const std::string &token): ParserError{"Missing token: \"" + token + "\""} {};
};

class MalformedExpression: public ParserError {
public:
    MalformedExpression(const std::string &expression): ParserError{"Malformed expression: \"" + expression + "\""} {};
};

/**
  *Transform a chain of trees (no childs) like the one the lexer function returns into a tree containing the entire expression
*/
class Parser {
    /**
      *The expressionTreeRoot should never contains a pointer on it,
      *because it could be used and freed in the calling program after the parser call.
      *Consider expressionTokens has const
     */
    Node* expressionTokens;
    
    // only used to avoid recalculating many times the root
    Node* expressionTreeRoot = new Node{Token::NullRoot};
    Node* expressionTree = expressionTreeRoot;
    Settings* settings;
    void goToParentBlock();
    bool isValidName(const std::string& str, size_t start, size_t end);
    bool isValidElementOrStyleName(const std::string& str);
    bool isValidClass(const std::string& str);
    bool isValidIdentifier(const std::string& str);
    bool isValidModifier(const std::string& str);
    void removeSpace();
    void removeLineReturn();
    void removeSpacesAndLineReturns();

    void parse();

    void parseSpace();
    void parseLineReturn();
    void parseOneLineComment();
    void parseMultiLineComment();
    void parseValue();
    void parseComma();
    void parseColon();
    void parseSemiColon();
    void parseOpeningParenthesis();
    void parseClosingParenthesis();
    void parseOpeningCurlyBracket();
    void parseClosingCurlyBracket();
    void parseString();
    void parseName();
    void parseUnit();
    void parseClass();
    void parseIdentifier();
    void parseModifier();

public:
    Parser(Node* expressionTokens, Settings* settings)
    : expressionTokens{expressionTokens}, settings{settings} {parse();};
    Node* getFinalTree() {return expressionTreeRoot;}
};

#endif // PARSER_HPP