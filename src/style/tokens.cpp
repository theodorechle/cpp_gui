#include <string>

#include "tokens.hpp"

std::string tokenToString(const Token &type) {
    std::string value;
    switch (type) {
        case Token::Name: value = "Name";break;
        case Token::Space: value = "Space";break;
        case Token::Function: value = "Function";break;
        case Token::Int: value = "Int";break;
        case Token::Float: value = "Float";break;
        case Token::String: value = "String";break;
        case Token::Bool: value = "Bool";break;
        case Token::Tuple: value = "Tuple";break;
        case Token::Unit: value = "Unit";break;
        case Token::OpeningParenthesis: value = "OpeningParenthesis";break;
        case Token::ClosingParenthesis: value = "ClosingParenthesis";break;
        case Token::OpeningCurlyBracket: value = "OpeningParenthesis";break;
        case Token::ClosingCurlyBracket: value = "ClosingParenthesis";break;
        case Token::Empty: value = "Empty";break;
        case Token::NullRoot: value = "NullRoot";break;
        default: value = "Unknown";break;
    }
    return value;
}

std::ostream& operator<<(std::ostream& o, const Token type) {
    o << tokenToString(type);
    return o;
}
