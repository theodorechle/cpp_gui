#include "tokenizer.hpp"
#include "units.hpp"

void Tokenizer::tokenizeSpace() {
    size_t i = 0;
    while (index+i < expressionLength && (expression[index+i] == ' ' || expression[index+i] == '\t')
        ) {
        i++;
    }
    if (i > 0) {
        expressionTree->appendNext(new Node{Token::Space});
        tokenized = true;
        index += i;
    }
}

void Tokenizer::tokenizeLineReturn() {
    size_t i = 0;
    while (index+i < expressionLength && expression[index+i] == '\n') {
        i++;
    }
    if (i > 0) {
        expressionTree->appendNext(new Node{Token::LineReturn});
        tokenized = true;
        index += i;
    }
}

void Tokenizer::tokenizeOneLineComment() {
    if (expression[index] != '/' || expression[index+1] != '/') return;
    expressionTree->appendNext(new Node{Token::OneLineComment});
    tokenized = true;
    index += 2;
}

void Tokenizer::tokenizeOpeningMultiLineComment() {
    if (expression[index] != '/' || expression[index+1] != '*') return;
    expressionTree->appendNext(new Node{Token::OpeningMultiLineComment});
    tokenized = true;
    index += 2;
}

void Tokenizer::tokenizeClosingMultiLineComment() {
    if (expression[index] != '*' || expression[index+1] != '/') return;
    expressionTree->appendNext(new Node{Token::ClosingMultiLineComment});
    tokenized = true;
    index += 2;
}

void Tokenizer::tokenizeString() {
    size_t i = 0;
    while (index + i < expressionLength && (
        expression[index+i] != ','
        && expression[index+i] != ';'
        && expression[index+i] != ':'
        && expression[index+i] != ' '
        && expression[index+i] != '\n'
        && expression[index+i] != '\t'
        && expression[index+i] != '{'
        && expression[index+i] != '}'
        && expression[index+i] != '('
        && expression[index+i] != ')'
        && expression[index+i] != '['
        && expression[index+i] != ']'
        )) {
        i++;
    }
    expressionTree->appendNext(new Node{Token::String, expression.substr(index, i)});
    index += i;
    tokenized = true;    
}

void Tokenizer::tokenizeInt() {
    if (!isdigit(expression[index])) return;
    size_t i = 1;
    while (index+i < expressionLength && isdigit(expression[index+i])) i++;
    expressionTree->appendNext(new Node{Token::Int, expression.substr(index, i)});
    index += i;
    tokenized = true;
}

void Tokenizer::tokenizeFloat() {
    bool dotFound = false;
    if (expression[index] == '.') dotFound = true;
    else if (!isdigit(expression[index])) return;
    size_t i = 1;
    while (index+i < expressionLength && (isdigit(expression[index+i]) || expression[index+i] == '.')) {
        if (expression[index+i] == '.') {
            if (!dotFound) dotFound = true;
            else return;
        }
        i++;
    }
    if (!dotFound) return;
    expressionTree->appendNext(new Node{Token::Float, expression.substr(index, i)});
    index += i;
    tokenized = true;
}

void Tokenizer::tokenizeBool() {
    if (expression.substr(index, TRUE.size()) == TRUE) {
        expressionTree->appendNext(new Node{Token::Bool, expression.substr(index, TRUE.size())});
        index += TRUE.size();
        tokenized = true;
    }
    else if (expression.substr(index, FALSE.size()) == FALSE) {
        expressionTree->appendNext(new Node{Token::Bool, expression.substr(index, FALSE.size())});
        index += FALSE.size();
        tokenized = true;
    }
}

void Tokenizer::tokenizeUnit() {
    std::unordered_map<std::string, function>::const_iterator map_it;
    size_t i;
    bool isEqual;

    for (map_it = UNITS.cbegin(); map_it != UNITS.cend(); map_it++) {
        isEqual = true;
        for (i = 0; i < map_it->first.size(); i++) {
            if (expression[index+i] != map_it->first[i]) {
                isEqual = false;
                break;
            }
        }
        if (isEqual) {
            expressionTree->appendNext(new Node{Token::Unit, expression.substr(index, i)});
            index += 1;
            tokenized = true;
            return;
        }
    }
    
}

void Tokenizer::tokenizeSpecialCharacters() {
    Token token;
    switch (expression[index]) {
    case '(':
        token = Token::OpeningParenthesis;
        break;
    case ')':
        token = Token::ClosingParenthesis;
        break;
    case '{':
        token = Token::OpeningCurlyBracket;
        break;
    case '}':
        token = Token::ClosingCurlyBracket;
        break;
    case ',':
        token = Token::Comma;
        break;
    case ':':
        token = Token::Colon;
        break;
    case ';':
        token = Token::SemiColon;
        break;
        break;
    default:
        return;
    }
    expressionTree->appendNext(new Node{token});
    index++;
    tokenized = true;
}

void Tokenizer::tokenize() {
    while (index < expressionLength) {
        tokenized = false;
        tokenizeSpace();
        if (!tokenized) tokenizeLineReturn();
        if (!tokenized) tokenizeOneLineComment();
        if (!tokenized) tokenizeOpeningMultiLineComment();
        if (!tokenized) tokenizeClosingMultiLineComment();
        if (!tokenized) tokenizeInt();
        if (!tokenized) tokenizeFloat();
        if (!tokenized) tokenizeBool();
        if (!tokenized) tokenizeUnit();
        if (!tokenized) tokenizeSpecialCharacters();
        if (!tokenized) tokenizeString();
        if (!tokenized) {
            delete expressionTree; // avoid memory leak
            throw UnknownValue(expression.substr(index));
        }
        if (settings->debug) std::cerr << tokenToString(expressionTree->getTokenType()) << ": '" << expressionTree->getValue() << "'" << std::endl;
        expressionTree = expressionTree->getNext();
    }
    // remove the NullRoot token at the start
    Node* nextList = firstNode->getNext();
    firstNode->setNext(nullptr);
    delete firstNode;
    firstNode = nextList;
}
