#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>

#include "node.hpp"
#include "settings.hpp"

class UnknownValue: public std::exception {
    std::string message;
public:
    UnknownValue(const std::string& value): message{"Error : Unknown value '" + value + "'"} {};
    const char* what() const noexcept override {return message.c_str();};
};

class Tokenizer {
    bool tokenized;
    size_t index=0;
    const std::string &expression;
    size_t expressionLength;
    Node *expressionTree = new Node{Token::NullRoot};
    Settings *settings;
public:
    Tokenizer(const std::string &expression, Settings *settings)
    : expression{expression}, expressionLength{expression.length()}, settings{settings} {
        tokenize();
    };
    void tokenize();
    void tokenizeSpace();
    void tokenizeName();
    void tokenizeString();
    void tokenizeInt();
    void tokenizeFloat();
    void tokenizeBool();
    void tokenizeUnit();
    void tokenizeSpecialCharacters();
    Node *getResult() {return expressionTree;}
};

#endif // TOKENIZER_HPP