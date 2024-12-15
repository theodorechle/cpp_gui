#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <unordered_map>
#include <string>
#include <iostream>

enum class Token {
    // Used only by the lexer
    OpeningParenthesis,
    ClosingParenthesis,
    OpeningCurlyBracket,
    ClosingCurlyBracket,
    Comma,
    Colon,
    SemiColon,
    OneLineComment,
    MultiLineComment,
    // Used only by the parser
    StyleBlock,
    BlockDeclaration,
    Declaration,
    BlockDefinition,
    DirectParent,
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
    /**
     * NullRoot is used only when an algorithm must start with an existing node
     * Nodes with this type will be ignored by the root function in node.h and the Parser::goToParentBlock function
     * (they return the NullRoot node's child, except when it's the only node)
    */
   NullRoot,
    // value types
    Int,
    Float,
    Bool,
    PercentageUnit,
    PixelUnit
};

const std::unordered_map<std::string, Token> UNITS {
    {"%", Token::PercentageUnit},
    {"px", Token::PixelUnit}
};

const std::string TRUE = "true";
const std::string FALSE = "false";

std::string tokenToString(const Token &type);

std::ostream& operator<<(std::ostream& o, const Token type);

#endif // TOKENS_HPP