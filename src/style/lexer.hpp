#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <vector>
#include <map>
#include <string>
#include <algorithm>

#include "node.hpp"
#include "settings.hpp"

#define MAX_ERROR_COMPLEMENTARY_INFOS_SIZE 20

class LexerError : public std::exception {
    std::string message;

public:
    LexerError(const std::string &message) : message{message} {}
    const char *what() const noexcept override { return message.c_str(); };
};

class UnknownValue : public LexerError {
public:
    UnknownValue(const std::string &value) : LexerError{"Error : Unknown value '" + value + "'"} {};
};

const std::map<char, Token> specialCharacters = {
    {'(', Token::OpeningParenthesis},
    {')', Token::ClosingParenthesis},
    {'{', Token::OpeningCurlyBracket},
    {'}', Token::ClosingCurlyBracket},
    {',', Token::Comma},
    {':', Token::Colon},
    {';', Token::SemiColon},
    {'>', Token::GreaterThan},
};

const std::vector<char> forbiddenFirstStringCharacters = {
    '[', ']'
};

// these characters are only allowed at the beginning of a string
const std::vector<char> allowedSpecialFirstStringCharacters = {
    ':', ' ', '#', '.'
};

class Lexer {
    bool lexed;
    size_t index = 0;
    const std::string &expression;
    size_t expressionLength;
    Node *firstNode = new Node(Token::NullRoot);
    Node *expressionTree = firstNode;
    Settings *settings;

public:
    Lexer(const std::string &expression, Settings *settings) : expression{expression}, expressionLength{expression.length()}, settings{settings} {
        lexe();
    };
    void lexe();
    void lexeSpace();
    void lexeLineReturn();
    void lexeOneLineComment();
    void lexeMultiLineComment();
    void lexeClosingMultiLineComment();
    void lexeString();
    void lexeInt();
    void lexeFloat();
    void lexeBool();
    void lexeUnit();
    void lexeSpecialCharacters();
    Node *getResult() { return firstNode; }
};

#endif // TOKENIZER_HPP