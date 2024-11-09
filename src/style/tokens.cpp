#include <string>

#include "tokens.hpp"

std::string tokenToString(const Token &type) {
    std::string value;
    switch (type) {
        case Token::Name: value = "Name";break;
        case Token::Space: value = "Space";break;
        case Token::Variable: value = "Variable";break;
        case Token::Number: value = "Number";break;
        case Token::Function: value = "Function";break;
        case Token::OpeningParenthesis: value = "OpeningParenthesis";break;
        case Token::ClosingParenthesis: value = "ClosingParenthesis";break;
        case Token::Plus: value = "Plus";break;
        case Token::Minus: value = "Minus";break;
        case Token::Times: value = "Times";break;
        case Token::ImplicitTimes: value = "ImplicitTimes";break;
        case Token::Slash: value = "Slash";break;
        case Token::DoubleTimes: value = "DoubleTimes";break;
        case Token::Caret: value = "Caret";break;
        case Token::Comma: value = "Comma";break;
        case Token::Bang: value = "Bang";break;
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
