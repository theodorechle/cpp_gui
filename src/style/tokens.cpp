#include <string>

#include "tokens.hpp"

std::string tokenToString(const Token &type) {
    switch (type) {
        // Used only by the tokenizer
        case Token::OpeningParenthesis: return "OpeningParenthesis";
        case Token::ClosingParenthesis: return "ClosingParenthesis";
        case Token::OpeningCurlyBracket: return "OpeningCurlyBracket";
        case Token::ClosingCurlyBracket: return "ClosingCurlyBracket";
        case Token::Comma: return "Comma";
        case Token::Colon: return "Colon";
        case Token::SemiColon: return "SemiColon";
        case Token::OneLineComment: return "OneLineComment";
        case Token::OpeningMultiLineComment: return "OpeningMultiLineComment";
        case Token::ClosingMultiLineComment: return "ClosingMultiLineComment";
        // Used only by the parser
        case Token::StyleBlock: return "StyleBlock";
        case Token::BlockPrototype: return "BlockPrototype";
        case Token::BlockDefinition: return "BlockDefinition";
        case Token::StyleName: return "StyleName";
        case Token::Function: return "Function";
        case Token::ElementName: return "ElementName";
        case Token::Tuple: return "Tuple";
        case Token::Assignment: return "Assignment";
        case Token::ArgSeparator: return "ArgSeparator";
        // Used by both the tokenizer and the parser
        case Token::Space: return "Space";
        case Token::LineReturn: return "LineReturn";
        case Token::Name: return "Name";
        case Token::Int: return "Int";
        case Token::Float: return "Float";
        case Token::String: return "String";
        case Token::Bool: return "Bool";
        case Token::Unit: return "Unit";
        case Token::Class: return "Class";
        case Token::Modifier: return "Modifier";
        case Token::Identifier: return "Identifier";
        case Token::Empty: return "Empty";
        case Token::NullRoot: return "NullRoot";
        default: return "Unknown";
    }
}

std::ostream& operator<<(std::ostream& o, const Token type) {
    o << tokenToString(type);
    return o;
}
