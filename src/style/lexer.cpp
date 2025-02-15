#include "lexer.hpp"

namespace style {

    void Lexer::lexeSpace() {
        size_t i = 0;
        while (index + i < expressionLength && (expression[index + i] == ' ' || expression[index + i] == '\t')) {
            i++;
        }
        if (i > 0) {
            parsedTree->appendNext(new Node(Token::Space));
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
        parsedTree->appendNext(new Node(Token::LineReturn));
        lexed = true;
        index += i;
    }

    void Lexer::lexeOneLineComment() {
        if (expression[index] != '/' || expression[index + 1] != '/') return;
        int i = 1;
        while (index + i + 1 < expression.size() && expression[index + i + 1] != '\n') {
            i++;
        }
        parsedTree->appendNext(new Node(Token::OneLineComment, expression.substr(index + 2, i - 1)));
        lexed = true;
        index += i + 1;
    }

    void Lexer::lexeMultiLineComment() {
        if (expression[index] != '/' || index + 1 == expression.size() || expression[index + 1] != '*') return;
        int i = 1;
        while (index + i + 2 < expression.size() && !(expression[index + i + 1] == '*' && expression[index + i + 2] == '/')) {
            i++;
        }
        if (index + i + 2 >= expression.size()) return;
        parsedTree->appendNext(new Node(Token::MultiLineComment, expression.substr(index + 2, i - 1)));
        lexed = true;
        index += i + 3;
    }

    void Lexer::lexePseudoName() {
        size_t i = 0;
        while (std::isalnum(expression[index + i])
               || std::find(PSEUDO_NAME_ALLOWED_SPECIAL_CHARACTERS.cbegin(), PSEUDO_NAME_ALLOWED_SPECIAL_CHARACTERS.cend(), expression[index + i])
                      != PSEUDO_NAME_ALLOWED_SPECIAL_CHARACTERS.cend()) {
            i++;
        }
        if (i == 0) return;
        parsedTree->appendNext(new Node(Token::PseudoName, expression.substr(index, i)));
        index += i;
        lexed = true;
    }

    void Lexer::lexeString() {
        if (expression[index] != '"') return;
        int i = 1;
        while (index + i + 1 < expression.size() && expression[index + i + 1] != '"') {
            i++;
        }
        if (index + i >= expression.size()) return;
        parsedTree->appendNext(new Node(Token::String, expression.substr(index + 1, i)));
        lexed = true;
        index += i + 2;
    }

    void Lexer::lexeInt() {
        if (!isdigit(expression[index])) return;
        int tmpSize;
        size_t i = 1;
        while (index + i < expressionLength && isdigit(expression[index + i]))
            i++;
        if (SPECIAL_CHARACTERS.find(expression[index + i]) == SPECIAL_CHARACTERS.cend() && expression[index + i] != ' '
            && expression[index + i] != '\n' && getUnit(i, &tmpSize) == Token::NullRoot)
            return; // not an int
        parsedTree->appendNext(new Node(Token::Int, expression.substr(index, i)));
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
        if (SPECIAL_CHARACTERS.find(expression[index + i]) == SPECIAL_CHARACTERS.cend() && expression[index + i] != ' '
            && expression[index + i] != '\n' && getUnit(i, &tmpSize) == Token::NullRoot)
            return; // not a float
        parsedTree->appendNext(new Node(Token::Float, expression.substr(index, i)));
        index += i;
        lexed = true;
    }

    void Lexer::lexeBool() {
        if (expression.substr(index, TRUE.size()) == TRUE) {
            parsedTree->appendNext(new Node(Token::Bool, expression.substr(index, TRUE.size())));
            index += TRUE.size();
            lexed = true;
        }
        else if (expression.substr(index, FALSE.size()) == FALSE) {
            parsedTree->appendNext(new Node(Token::Bool, expression.substr(index, FALSE.size())));
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
        parsedTree->appendNext(new Node(unit));
    }

    void Lexer::lexeSpecialCharacters() {
        std::map<char, Token>::const_iterator specialCharIt = SPECIAL_CHARACTERS.find(expression[index]);
        if (specialCharIt == SPECIAL_CHARACTERS.cend()) return;
        parsedTree->appendNext(new Node(specialCharIt->second));
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
            if (!lexed) lexeString();
            if (!lexed) lexeFloat();
            if (!lexed) lexeInt();
            if (!lexed) lexeBool();
            if (!lexed) lexeUnit();
            if (!lexed) lexePseudoName();
            if (!lexed) lexeSpecialCharacters();
            if (!lexed) {
                delete firstNode;
                firstNode = nullptr;
                throw UnknownValue(expression.substr(index, MAX_ERROR_COMPLEMENTARY_INFOS_SIZE));
            }
            #ifdef DEBUG_LEXER
            std::cerr << tokenToString(parsedTree->getToken()) << ": '" << parsedTree->getValue() << "'\n";
            #endif
            parsedTree = parsedTree->getNext();
        }
        // remove the NullRoot token at the start
        Node *nextList = firstNode->getNext();
        firstNode->setNext(nullptr);
        delete firstNode;
        firstNode = nextList;
    }

} // namespace style
