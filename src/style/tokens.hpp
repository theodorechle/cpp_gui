#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <unordered_map>

enum class Token {
    Name, // Only used by the parser
    Function,
    Int,
    Float,
    String,
    Bool,
    Tuple,
    OpeningParenthesis,
    ClosingParenthesis,
    Space,
    Unit,
    Empty,
    /**NullRoot is used only when an algorithm must start with an existing node
     * Nodes with this type will be ignored by the root and Parser::getRootOrStopBeforeParenthesis functions
     * (they return the NullRoot node's child, except if it's the only node)
    */
   NullRoot
};

std::string TRUE = "true";
std::string FALSE = "false";

std::string tokenToString(const Token &type);

std::ostream& operator<<(std::ostream& o, const Token type);

#endif // TOKENS_HPP