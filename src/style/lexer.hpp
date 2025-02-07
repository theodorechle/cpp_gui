#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "node.hpp"
#include "settings.hpp"

constexpr int MAX_ERROR_COMPLEMENTARY_INFOS_SIZE = 20;

namespace style {

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

    const std::map<char, Token> SPECIAL_CHARACTERS = {
        {'(', Token::OpeningParenthesis},
        {')', Token::ClosingParenthesis},
        {'{', Token::OpeningCurlyBracket},
        {'}', Token::ClosingCurlyBracket},
        {',', Token::Comma},
        {':', Token::Colon},
        {';', Token::SemiColon},
        {'>', Token::GreaterThan},
        {'#', Token::Sharp},
        {'.', Token::Dot},
        {'&', Token::Ampersand},
        {'@', Token::At}
    };

    const std::vector<char> PSEUDO_NAME_ALLOWED_SPECIAL_CHARACTERS = {'-', '_'};

    class Lexer {
        bool lexed;
        size_t index = 0;
        const std::string &expression;
        size_t expressionLength;
        Node *firstNode = new Node(Token::NullRoot);
        Node *parsedTree = firstNode;
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
        void lexePseudoName();
        void lexeString();
        void lexeInt();
        void lexeFloat();
        void lexeBool();
        Token getUnit(int expressionIndex, int *size);
        void lexeUnit();
        void lexeSpecialCharacters();
        Node *getResult() { return firstNode; }
    };

} // namespace style

#endif // TOKENIZER_HPP