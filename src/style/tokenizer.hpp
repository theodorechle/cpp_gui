#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>

#include "node.hpp"
#include "settings.hpp"

#define MAX_ERROR_COMPLEMENTARY_INFOS_SIZE 20

class TokenizerError: public std::exception {
    std::string message;
public:
    TokenizerError(const std::string& message): message{message} {}
    const char* what() const noexcept override {return message.c_str();};
};

class UnknownValue: public TokenizerError {
public:
    UnknownValue(const std::string& value): TokenizerError{"Error : Unknown value '" + value + "'"} {};
};

class Tokenizer {
    bool tokenized;
    size_t index=0;
    const std::string &expression;
    size_t expressionLength;
    Node* firstNode = new Node{Token::NullRoot};
    Node* expressionTree = firstNode;
    Settings* settings;
public:
    Tokenizer(const std::string &expression, Settings* settings)
    : expression{expression}, expressionLength{expression.length()}, settings{settings} {
        tokenize();
    };
    void tokenize();
    void tokenizeSpace();
    void tokenizeLineReturn();
    void tokenizeOneLineComment();
    void tokenizeMultiLineComment();
    void tokenizeClosingMultiLineComment();
    void tokenizeString();
    void tokenizeInt();
    void tokenizeFloat();
    void tokenizeBool();
    void tokenizeUnit();
    void tokenizeSpecialCharacters();
    Node* getResult() {return firstNode;}
};

#endif // TOKENIZER_HPP