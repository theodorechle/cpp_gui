#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <unordered_map>

enum class Token {
    // Used only by the tokenizer
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
    Function,
    ElementName,
    Tuple,
    String,
    Assignment,
    ArgSeparator,
    Name,
    Class,
    Modifier,
    Identifier,
    // Used by both the tokenizer and the parser
    Space,
    LineReturn,
    Int,
    Float,
    Bool,
    Unit,
    Empty,
    /**NullRoot is used only when an algorithm must start with an existing node
      *Nodes with this type will be ignored by the root function in node.h and the Parser::goToParentBlock function
      *(they return the NullRoot node's child, except when it's the only node)
    */
   NullRoot
};

const std::string TRUE = "true";
const std::string FALSE = "false";

std::string tokenToString(const Token &type);

std::ostream& operator<<(std::ostream& o, const Token type);

#endif // TOKENS_HPP