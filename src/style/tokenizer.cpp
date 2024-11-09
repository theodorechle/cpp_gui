#include "tokenizer.hpp"
#include "addition.hpp"

void Tokenizer::tokenizeSpace() {
    size_t i = 0;
    while (index+i < expressionLength && isspace(expression[index+i])) {
        i++;
    }
    if (i > 0) {
        expressionTree->appendNext(new Node{Token::Space});
        tokenized = true;
        index += i;
    }
}

void Tokenizer::tokenizeName() {
    if (!isalpha(expression[index]) ||
            expression[index] == '_') {
        return;
    }
    size_t i = 1;
    while (index+i < expressionLength && (isalnum(expression[index+i]) ||
            expression[index+i] == '_')) {
        i++;
    }
    expressionTree->appendNext(new Node{Token::Name, expression.substr(index, i)});
    index += i;
    tokenized = true;
}

void Tokenizer::tokenizeNumber() {
    bool dotFound = false;
    if (expression[index] == '.') dotFound = true;
    else if (!isdigit(expression[index])) return;
    size_t i = 1;
    while (index+i < expressionLength && (isdigit(expression[index+i]) ||
            expression[index+i] == '_' || expression[index+i] == '.')) {
        if (expression[index+i] == '.') {
            if (!dotFound) dotFound = true;
            else return;
        }
        i++;
    }
    expressionTree->appendNext(new Node{Token::Number, expression.substr(index, i)});
    index += i;
    tokenized = true;
}

void Tokenizer::tokenizeSpecialCharacters() {
    if (index + 1 < expressionLength && expression[index] == '*' && expression[index+1] == '*') {
        expressionTree->appendNext(new Node{Token::DoubleTimes});
        index += 2;
        tokenized = true;
        return;
    }
    bool found = false;
    switch (expression[index]) {
    case '+':
        expressionTree->appendNext(new Addition);
        found = true;
        break;
    }
    if (found) {
        index++;
        tokenized = true;
        return;
    }

    Token token;
    switch (expression[index]) {
    case '(':
        token = Token::OpeningParenthesis;
        break;
    case ')':
        token = Token::ClosingParenthesis;
        break;
    case '-':
        token = Token::Minus;
        break;
    case '*':
        token = Token::Times;
        break;
    case '/':
        token = Token::Slash;
        break;
    case '^':
        token = Token::Caret;
        break;
    case ',':
        token = Token::Comma;
        break;
    case '!':
        token = Token::Bang;
        break;    
    default:
        token = Token::Empty;
        break;
    }
    if (token == Token::Empty) return;
    expressionTree->appendNext(new Node{token});
    index++;
    tokenized = true;
}

void Tokenizer::tokenize() {
    while (index < expressionLength) {
        tokenized = false;
        tokenizeSpace();
        if (!tokenized) tokenizeNumber();
        if (!tokenized) tokenizeName();
        if (!tokenized) tokenizeSpecialCharacters();
        if (!tokenized) {
            delete expressionTree;
            throw UnknownValue(expression.substr(index));
        }
    }
    Node *nextList = expressionTree->getNext();
    expressionTree->setNext(nullptr);
    delete expressionTree;
    expressionTree = nextList;
}
