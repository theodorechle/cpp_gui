#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <unordered_map>

enum class Token {
    Name, // Only used by the parser
    Space,
    Function,
    Int,
    Float,
    String,
    Bool,
    Tuple,
    Unit,
    OpeningParenthesis,
    ClosingParenthesis,
    OpeningCurlyBracket,
    ClosingCurlyBracket,
    Class,
    Modifier,
    Identifier,
    Comma,
    SemiColon,
    Empty,
    /**NullRoot is used only when an algorithm must start with an existing node
     * Nodes with this type will be ignored by the root function in node.h and the Parser::getParentBlock function
     * (they return the NullRoot node's child, except when it's the only node)
    */
   NullRoot
};

std::string TRUE = "true";
std::string FALSE = "false";

std::string tokenToString(const Token &type);

std::ostream& operator<<(std::ostream& o, const Token type);

#endif // TOKENS_HPP