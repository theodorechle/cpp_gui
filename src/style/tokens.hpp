#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <iostream>
#include <string>
#include <map>

enum class Token {
    // Used only by the lexer
    OpeningParenthesis,
    ClosingParenthesis,
    OpeningCurlyBracket,
    ClosingCurlyBracket,
    Comma,
    Colon,
    SemiColon,
    GreaterThan,
    OneLineComment,
    MultiLineComment,
    Sharp,
    Dot,
    // Used only by the parser
    StyleBlock,
    BlockDeclaration,
    Declaration,
    BlockDefinition,
    DirectParent,
    AnyParent,
    StyleName,
    Assignment,
    ArgSeparator,
    ElementName,
    Name,
    Class,
    Modifier,
    Identifier,
    // value types
    Tuple,
    String,
    Function,
    // Used by both the lexer and the parser
    Space,
    LineReturn,
    Empty,
    // NullRoot is used only when an algorithm must start with an existing node
    NullRoot,
    // value types
    Int,
    Float,
    Bool,
    PercentageUnit,
    PixelUnit,
    Hex,
    None,
    Auto
};

const std::map<std::string, Token> UNITS{
    {"%", Token::PercentageUnit},
    {"px", Token::PixelUnit}};

const std::string TRUE = "true";
const std::string FALSE = "false";

std::string tokenToString(const Token &type);

std::ostream &operator<<(std::ostream &o, const Token type);

#endif // TOKENS_HPP