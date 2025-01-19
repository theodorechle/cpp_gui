#include "lexer.hpp"

void Lexer::lexeSpace() {
    size_t i = 0;
    while (index + i < expressionLength && (expression[index + i] == ' ' || expression[index + i] == '\t')) {
        i++;
    }
    if (i > 0) {
        expressionTree->appendNext(new Node(Token::Space));
        lexed = true;
        index += i;
    }
}

void Lexer::lexeLineReturn() {
    size_t i = 0;
    while (index + i < expressionLength && expression[index + i] == '\n') {
        i++;
    }
    if (i == 0) return;
    expressionTree->appendNext(new Node(Token::LineReturn));
    lexed = true;
    index += i;
}

void Lexer::lexeOneLineComment() {
    if (expression[index] != '/' || expression[index + 1] != '/') return;
    int i = 1;
    while (index + i + 1 < expression.size() && expression[index + i + 1] != '\n') {
        i++;
    }
    expressionTree->appendNext(new Node{Token::OneLineComment, expression.substr(index + 2, i - 1)});
    lexed = true;
    index += i + 1;
}

void Lexer::lexeMultiLineComment() {
    if (expression[index] != '/' || expression[index + 1] != '*') return;
    int i = 1;
    while (index + i + 2 < expression.size() && expression[index + i + 1] != '*' && expression[index + i + 2] != '/') {
        i++;
    }
    if (index + i + 2 >= expression.size()) throw LexerError("Multiline comment opened but never closed");
    expressionTree->appendNext(new Node{Token::MultiLineComment, expression.substr(index + 2, i - 1)});
    lexed = true;
    index += i + 3;
}

void Lexer::lexeString() {
    size_t i = 0;
    // check that the string does not contains a special character who is part of the syntax or a forbidden character
    while (index + i < expressionLength && SPECIAL_CHARACTERS.find(expression[index + i]) == SPECIAL_CHARACTERS.cend()
           && std::find(FORBIDDEN_STRING_CHARACTERS.cbegin(), FORBIDDEN_STRING_CHARACTERS.cend(), expression[index + i])
                  == FORBIDDEN_STRING_CHARACTERS.cend()) {
        i++;
    }
    if (i == 0) return;
    expressionTree->appendNext(new Node{Token::String, expression.substr(index, i)});
    index += i;
    lexed = true;
}

void Lexer::lexeHex() {
    if (expression[index] != '#') return;
    size_t i = 1;
    while (std::isxdigit(expression[index + i])) {
        i++;
    }
    if (i == 1) return;
    expressionTree->appendNext(new Node(Token::Hex, expression.substr(index + 1, i - 1)));
    index += i;
    lexed = true;
}

void Lexer::lexeInt() {
    if (!isdigit(expression[index])) return;
    int tmpSize;
    size_t i = 1;
    while (index + i < expressionLength && isdigit(expression[index + i]))
        i++;
    if (SPECIAL_CHARACTERS.find(expression[index + i]) == SPECIAL_CHARACTERS.cend() && expression[index + i] != ' ' && expression[index + i] != '\n'
        && getUnit(i, &tmpSize) == Token::NullRoot)
        return; // not an int
    expressionTree->appendNext(new Node{Token::Int, expression.substr(index, i)});
    index += i;
    lexed = true;
}

void Lexer::lexeFloat() {
    int tmpSize;
    bool dotFound = false;
    size_t i = 0;
    while (index + i < expressionLength) {
        if (expression[index + i] == '.') {
            if (!dotFound) dotFound = true;
            else return;
        }
        else if (!isdigit(expression[index + i])) return;
        i++;
    }
    if (!dotFound || i < 2) return; // need at least one int (0-9) and a dot
    if (SPECIAL_CHARACTERS.find(expression[index + i]) == SPECIAL_CHARACTERS.cend() && expression[index + i] != ' ' && expression[index + i] != '\n'
        && getUnit(i, &tmpSize) == Token::NullRoot)
        return; // not a float
    expressionTree->appendNext(new Node{Token::Float, expression.substr(index, i)});
    index += i;
    lexed = true;
}

void Lexer::lexeBool() {
    if (expression.substr(index, TRUE.size()) == TRUE) {
        expressionTree->appendNext(new Node{Token::Bool, expression.substr(index, TRUE.size())});
        index += TRUE.size();
        lexed = true;
    }
    else if (expression.substr(index, FALSE.size()) == FALSE) {
        expressionTree->appendNext(new Node{Token::Bool, expression.substr(index, FALSE.size())});
        index += FALSE.size();
        lexed = true;
    }
}

Token Lexer::getUnit(int expressionIndex, int *size) {
    size_t i;
    bool equal;
    for (std::pair<std::string, Token> unit : UNITS) {
        equal = true;
        for (i = 0; i < unit.first.size(); i++) {
            // std::cerr << expression[index + expressionIndex + i] << ", " << unit.first[i] << "\n";
            if (expression[index + expressionIndex + i] != unit.first[i]) {
                equal = false;
                break;
            }
        }
        if (equal) {
            *size = i;
            return unit.second;
        }
    }
    *size = 0;
    return Token::NullRoot;
}

void Lexer::lexeUnit() {
    int size;
    Token unit = getUnit(0, &size);
    if (unit == Token::NullRoot) return;
    index += size;
    lexed = true;
    expressionTree->appendNext(new Node(unit));
}

void Lexer::lexeSpecialCharacters() {
    std::map<char, Token>::const_iterator specialCharIt = SPECIAL_CHARACTERS.find(expression[index]);
    if (specialCharIt == SPECIAL_CHARACTERS.cend()) return;
    expressionTree->appendNext(new Node(specialCharIt->second));
    index++;
    lexed = true;
}

void Lexer::lexe() {
    while (index < expressionLength) {
        lexed = false;
        lexeSpace();
        if (!lexed) lexeLineReturn();
        if (!lexed) lexeOneLineComment();
        if (!lexed) lexeMultiLineComment();
        if (!lexed) lexeHex();
        if (!lexed) lexeFloat();
        if (!lexed) lexeInt();
        if (!lexed) lexeBool();
        if (!lexed) lexeUnit();
        if (!lexed) lexeString();
        if (!lexed) lexeSpecialCharacters();
        if (!lexed) {
            delete expressionTree; // avoid memory leak
            throw UnknownValue(expression.substr(index, MAX_ERROR_COMPLEMENTARY_INFOS_SIZE));
        }
        if (settings->debug) std::cerr << tokenToString(expressionTree->getTokenType()) << ": '" << expressionTree->getValue() << "'\n";
        expressionTree = expressionTree->getNext();
    }
    // remove the NullRoot token at the start
    Node *nextList = firstNode->getNext();
    firstNode->setNext(nullptr);
    delete firstNode;
    firstNode = nextList;
}
