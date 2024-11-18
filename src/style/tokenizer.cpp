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
    int i = 1;
    while (index + i + 1 < expression.size() && expression[index+i+1] != '\n') {
        i++;
    }
    expressionTree->appendNext(new Node{Token::OneLineComment, expression.substr(index+2, i-1)});
    tokenized = true;
    index += i + 1;
}

void Tokenizer::tokenizeMultiLineComment() {
    if (expression[index] != '/' || expression[index+1] != '*') return;
    int i = 1;
    while (index + i + 2 < expression.size() && expression[index+i+1] != '*' && expression[index+i+2] != '/') {
        i++;
    }
    expressionTree->appendNext(new Node{Token::MultiLineComment, expression.substr(index+2, i-2)});
    tokenized = true;
    index += i + 3;
}

void Tokenizer::tokenizeString() {
    if (expression[index] == ','
        || expression[index] == ';'
        || expression[index] == ' '
        || expression[index] == '\n'
        || expression[index] == '\t'
        || expression[index] == '{'
        || expression[index] == '}'
        || expression[index] == '('
        || expression[index] == ')'
        || expression[index] == '['
        || expression[index] == ']') return;
    
    size_t i = 1;
    while (index + i < expressionLength && (
        expression[index+i] != ','
        && expression[index+i] != ';'
        && expression[index+i] != ':'
        && expression[index+i] != '#'
        && expression[index+i] != '.'
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
    if (!isalpha(expression[index]) && i == 1) return;
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
    size_t i = 0;
    while (index+i < expressionLength) {
        if (expression[index+i] == '.') {
            if (!dotFound) dotFound = true;
            else return;
        }
        else if (!isdigit(expression[index+i])) return;
        i++;
    }
    if (!dotFound || i < 2) return; // < 2 because at least one int (0-9) and a dot
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
        if (!tokenized) tokenizeMultiLineComment();
        if (!tokenized) tokenizeInt();
        if (!tokenized) tokenizeFloat();
        if (!tokenized) tokenizeBool();
        if (!tokenized) tokenizeUnit();
        if (!tokenized) tokenizeString();
        if (!tokenized) tokenizeSpecialCharacters();
        if (!tokenized) {
            delete expressionTree; // avoid memory leak
            throw UnknownValue(expression.substr(index, MAX_ERROR_COMPLEMENTARY_INFOS_SIZE));
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
