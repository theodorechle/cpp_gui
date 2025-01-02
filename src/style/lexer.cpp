#include "lexer.hpp"

void Lexer::lexeSpace() {
    size_t i = 0;
    while (index + i < expressionLength && (expression[index + i] == ' ' || expression[index + i] == '\t')) {
        i++;
    }
    if (i > 0) {
        expressionTree->appendNext(new Node{Token::Space});
        lexed = true;
        index += i;
    }
}

void Lexer::lexeLineReturn() {
    size_t i = 0;
    while (index + i < expressionLength && expression[index + i] == '\n') {
        i++;
    }
    if (i > 0) {
        expressionTree->appendNext(new Node{Token::LineReturn});
        lexed = true;
        index += i;
    }
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
    expressionTree->appendNext(new Node{Token::MultiLineComment, expression.substr(index + 2, i - 1)});
    lexed = true;
    index += i + 3;
}

void Lexer::lexeString() {
    if (std::find(allowedSpecialFirstStringCharacters.cbegin(), allowedSpecialFirstStringCharacters.cend(), expression[index])
        == allowedSpecialFirstStringCharacters.cend()) {
        if (specialCharacters.find(expression[index]) != specialCharacters.cend()
            || std::find(forbiddenFirstStringCharacters.cbegin(), forbiddenFirstStringCharacters.cend(), expression[index])
                   != forbiddenFirstStringCharacters.cend())
            return;
    }
    size_t i = 1;
    while (index + i < expressionLength && specialCharacters.find(expression[index + i]) == specialCharacters.cend()
           && std::find(allowedSpecialFirstStringCharacters.cbegin(), allowedSpecialFirstStringCharacters.cend(), expression[index + i])
                  == allowedSpecialFirstStringCharacters.cend()) {
        i++;
    }
    if (!isalpha(expression[index]) && i == 1) return;
    expressionTree->appendNext(new Node{Token::String, expression.substr(index, i)});
    index += i;
    lexed = true;
}

void Lexer::lexeInt() {
    if (!isdigit(expression[index])) return;
    size_t i = 1;
    while (index + i < expressionLength && isdigit(expression[index + i]))
        i++;
    expressionTree->appendNext(new Node{Token::Int, expression.substr(index, i)});
    index += i;
    lexed = true;
}

void Lexer::lexeFloat() {
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
    if (!dotFound || i < 2) return; // < 2 because at least one int (0-9) and a dot
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

void Lexer::lexeUnit() {
    std::unordered_map<std::string, Token>::const_iterator map_it;
    size_t i;
    bool isEqual;

    for (map_it = UNITS.cbegin(); map_it != UNITS.cend(); map_it++) {
        isEqual = true;
        for (i = 0; i < map_it->first.size(); i++) {
            if (expression[index + i] != map_it->first[i]) {
                isEqual = false;
                break;
            }
        }
        if (isEqual) {
            expressionTree->appendNext(new Node{map_it->second});
            index += i;
            lexed = true;
            return;
        }
    }
}

void Lexer::lexeSpecialCharacters() {
    std::map<char, Token>::const_iterator specialCharIt = specialCharacters.find(expression[index]);
    if (specialCharIt != specialCharacters.cend()) {
        expressionTree->appendNext(new Node{specialCharIt->second});
        index++;
        lexed = true;
    }
}

void Lexer::lexe() {
    while (index < expressionLength) {
        lexed = false;
        lexeSpace();
        if (!lexed) lexeLineReturn();
        if (!lexed) lexeOneLineComment();
        if (!lexed) lexeMultiLineComment();
        if (!lexed) lexeInt();
        if (!lexed) lexeFloat();
        if (!lexed) lexeBool();
        if (!lexed) lexeUnit();
        if (!lexed) lexeString();
        if (!lexed) lexeSpecialCharacters();
        if (!lexed) {
            delete expressionTree; // avoid memory leak
            throw UnknownValue(expression.substr(index, MAX_ERROR_COMPLEMENTARY_INFOS_SIZE));
        }
        if (settings->debug) std::cerr << tokenToString(expressionTree->getTokenType()) << ": '" << expressionTree->getValue() << "'" << std::endl;
        expressionTree = expressionTree->getNext();
    }
    // remove the NullRoot token at the start
    Node *nextList = firstNode->getNext();
    firstNode->setNext(nullptr);
    delete firstNode;
    firstNode = nextList;
}
