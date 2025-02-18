#include "parser.hpp"

using namespace std;

namespace style {

    bool Parser::isValidName(const string &str, size_t start, size_t end) {
        size_t i;
        end = min(end, str.size());
        for (i = start; i < end - 1; i++) {
            if (!isalpha(str[i]) && str[i] != '_' && str[i] != '-') return false;
        }
        if (!isalpha(str[i]) && str[i] != '_') return false;
        return true;
    }

    bool Parser::isValidElementOrRuleName(const string &str) {
        if (!isalpha(str[0])) return false;
        return str.size() == 1 || isValidName(str, 1, str.size());
    }

    bool Parser::isWhiteSpace(Token token) { return (token == Token::Space || token == Token::LineBreak); }

    bool Parser::isComponentRelation(Token token) { return (token == Token::AnyParent || token == Token::DirectParent); }

    void Parser::removeSpace() {
        Node *lastChild = parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->getToken() == Token::Space) parsedTree->deleteSpecificChild(lastChild);
    }

    void Parser::removeLineReturn() {
        Node *lastChild = parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->getToken() == Token::LineBreak) parsedTree->deleteSpecificChild(lastChild);
    }

    void Parser::removeWhiteSpaces() {
        Node *lastChild = parsedTree->getLastChild();
        while (lastChild != nullptr && (lastChild->getToken() == Token::Space || lastChild->getToken() == Token::LineBreak)) {
            parsedTree->deleteSpecificChild(lastChild);
            lastChild = parsedTree->getLastChild();
        }
    }

    void Parser::parse() {
        try {
            while (currentToken != nullptr) {
#ifdef DEBUG
                cerr << "\nActual token : " << tokenToString(currentToken->getToken()) << ": '" << currentToken->getValue() << "'" << "\n";
#endif
                switch (currentToken->getToken()) {
                case Token::Space:
                    parseSpace();
                    break;
                case Token::LineBreak:
                    parseLineReturn();
                    break;
                case Token::OneLineComment:
                    parseOneLineComment();
                    break;
                case Token::MultiLineComment:
                    parseMultiLineComment();
                    break;
                case Token::String:
                    parseString();
                    break;
                case Token::Int:
                case Token::Float:
                case Token::Bool:
                    parseValue();
                    break;
                case Token::PseudoName:
                    parsePseudoName();
                    break;
                case Token::Comma:
                    parseComma();
                    break;
                case Token::Colon:
                    parseColon();
                    break;
                case Token::SemiColon:
                    parseSemiColon();
                    break;
                case Token::Sharp:
                    parseSharp();
                    break;
                case Token::Dot:
                    parseDot();
                    break;
                case Token::Ampersand:
                    parseAmpersand();
                    break;
                case Token::At:
                    parseAt();
                    break;
                case Token::Star:
                    parseStar();
                    break;
                case Token::GreaterThan:
                    parseGreatherThan();
                    break;
                case Token::PercentageUnit:
                case Token::PixelUnit:
                    parseUnit();
                    break;
                case Token::OpeningParenthesis:
                    parseOpeningParenthesis();
                    break;
                case Token::ClosingParenthesis:
                    parseClosingParenthesis();
                    break;
                case Token::OpeningCurlyBracket:
                    parseOpeningCurlyBracket();
                    break;
                case Token::ClosingCurlyBracket:
                    parseClosingCurlyBracket();
                    break;
                default:
                    throw UnknownToken(*currentToken);
                }
#ifdef DEBUG
                cerr << "Root :\n";
                expressionTreeRoot->display(cerr);
                cerr << "\n";
#endif
                currentToken = currentToken->getNext();
            }
            removeWhiteSpaces();
#ifdef DEBUG
            cerr << "Final parsed tree :\n";
            expressionTreeRoot->display(cerr);
            cerr << "\n";
#endif
        }
        catch (const ParserException &) {
            parsedTree = nullptr;
            delete expressionTreeRoot;
            throw;
        }
    }

    void Parser::parseSpace() {
        Node *lastChild;
        if (parsedTree->getToken() == Token::Declaration) {
            if (parsedTree->getNbChilds() > 0) {
                lastChild = parsedTree->getLastChild();
                if (lastChild == nullptr || !isComponentRelation(lastChild->getToken())) parsedTree->appendChild(new Node(Token::AnyParent));
            }
        }
        else parsedTree->appendChild(currentToken->copyNode());
    }

    void Parser::parseLineReturn() {
        Token token = parsedTree->getToken();
        if (token != Token::NullRoot && token != Token::BlockDefinition) {
            throw MalformedExpression("A line return can only be between blocks declarations and between assignments");
        }
        parsedTree->appendChild(currentToken->copyNode());
    }

    void Parser::parseOneLineComment() {}

    void Parser::parseMultiLineComment() {}

    void Parser::parseValue() {
        removeSpace();

        Node *lastChild;
        if (parsedTree->getToken() != Token::Assignment) {
            if (parsedTree->getToken() != Token::Tuple && parsedTree->getToken() != Token::Function)
                throw MalformedExpression(
                    "An int|float|bool|string must follow an assignment symbol or be inside of a tuple or a function parameter");
            lastChild = parsedTree->getLastChild();
            if (lastChild != nullptr && lastChild->getToken() != Token::ArgSeparator)
                throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
            parsedTree->deleteSpecificChild(lastChild);
            parsedTree->appendChild(currentToken->copyNode());
        }
        else {
            if (parsedTree->getNbChilds() > 1) throw MalformedExpression("Can only have one rvalue in an assignment");
            parsedTree->appendChild(currentToken->copyNode());
        }
    }

    void Parser::parseComma() {
        removeSpace();
        switch (parsedTree->getToken()) {
        case Token::Tuple:
            parsedTree->appendChild(new Node(Token::ArgSeparator));
            break;
        case Token::Declaration:
            parsedTree = parsedTree->getParent()->appendChild(new Node(Token::Declaration));
            break;
        default:
            throw MalformedExpression("A comma must be inside of a tuple");
        }
    }

    void Parser::parseColon() {
        removeSpace();

        Node *lastChild = parsedTree->getLastChild();
        Node *newChild;
        if (parsedTree->getToken() == Token::BlockDefinition && lastChild != nullptr && lastChild->getToken() == Token::Name) {
            lastChild->setToken(Token::RuleName);
            newChild = new Node(Token::Assignment);
            newChild->appendChild(lastChild->copyNodeWithChilds());
            parsedTree->replaceChild(lastChild, newChild);
            parsedTree = newChild;
        }
        else if (currentToken->getNext()->getToken() == Token::PseudoName) {
            currentToken = currentToken->getNext();
            parseModifier();
        }
        else throw MalformedExpression("A colon must be inside of a style block");
    }

    void Parser::parseSemiColon() {
        removeSpace();
        if (parsedTree->getToken() == Token::Assignment || parsedTree->getToken() == Token::Import) parsedTree = parsedTree->getParent();
        else throw MalformedExpression("A semi-colon must be at the end of an assignment");
    }

    void Parser::parseSharp() {
        Node *lastChild;
        removeSpace();
        currentToken = currentToken->getNext();
        if (parsedTree->getToken() != Token::Assignment) {
            if (parsedTree->getToken() != Token::Tuple && parsedTree->getToken() != Token::Function) {
                parseIdentifier();
                return;
            }
            lastChild = parsedTree->getLastChild();
            if (lastChild != nullptr && lastChild->getToken() != Token::ArgSeparator) {
                parseIdentifier();
                return;
            }
            if (currentToken->getToken() != Token::PseudoName) return;
            parsedTree->deleteSpecificChild(lastChild);
            parsedTree->appendChild(new Node{Token::Hex, currentToken->getValue()});
        }
        else {
            if (currentToken->getToken() != Token::PseudoName) return;
            if (parsedTree->getNbChilds() > 1) throw MalformedExpression("Can only have one rvalue in an assignment");
            parsedTree->appendChild(new Node{Token::Hex, currentToken->getValue()});
        }
    }

    void Parser::parseDot() {
        removeSpace();
        if (currentToken->getNext()->getToken() == Token::PseudoName) {
            currentToken = currentToken->getNext();
            parseClass();
        }
        else throw MalformedExpression("Illegal '.' placement");
    }

    void Parser::parseAmpersand() {
        Node *lastChild;
        Node *lastChildCopy = nullptr;
        Token token = parsedTree->getToken();
        if (token == Token::NullRoot || token == Token::BlockDefinition) {
            lastChild = parsedTree->getLastChild();
            if (lastChild != nullptr) {
                if (isWhiteSpace(lastChild->getToken())) {
                    removeWhiteSpaces();
                    lastChild = nullptr;
                }
                else if (lastChild->getToken() == Token::Name) lastChildCopy = new Node{Token::ElementName, lastChild->getValue()};
                else if (lastChild->getToken() == Token::AnyParent)
                    ; // do nothing, just ensure the node is being removed without being copied before
                else lastChildCopy = lastChild->copyNodeWithChilds();
                parsedTree->deleteSpecificChild(lastChild);
            }
            else {
                if (token == Token::NullRoot)
                    throw MissingToken("A '&' token must not be the first token of a block declaration if not a nested block");
            }
            parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock));
            parsedTree = parsedTree->appendChild(new Node(Token::BlockDeclaration));
            parsedTree = parsedTree->appendChild(new Node(Token::Declaration));
            parsedTree->appendChild(lastChildCopy);
            parsedTree->appendChild(new Node(Token::SameElement));
        }
        else if (token == Token::Declaration) {
            lastChild = parsedTree->getLastChild();
            if (lastChild->getToken() == Token::AnyParent) {
                parsedTree->deleteSpecificChild(lastChild);
            }
            parsedTree->appendChild(new Node(Token::SameElement));
        }
        else
            throw MalformedExpression("A same element relation ('&') must be before a style block opening and at the root level of the style file or "
                                      "inside an other style block");
    }

    void Parser::parseAt() {
        if (parsedTree->getToken() != Token::NullRoot) throw MalformedExpression("A '@' (at) token must be on the root level");
        currentToken = currentToken->getNext();
        if (currentToken == nullptr) throw MalformedExpression("A '@' (at) token must not be alone");
        if (currentToken->getToken() == Token::PseudoName && currentToken->getValue() == "import")
            parsedTree = parsedTree->appendChild(new Node{Token::Import});
        else throw MalformedExpression("Invalid '@' (at) placement");
    }

    void Parser::parseStar() {
        Token token = parsedTree->getToken();
        if (token == Token::NullRoot || token == Token::BlockDefinition) {
            removeSpace();
        }
        else if (token == Token::Declaration) {
            removeSpace();
            parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock));
            parsedTree = parsedTree->appendChild(new Node(Token::BlockDeclaration));
            parsedTree = parsedTree->appendChild(new Node(Token::Declaration));
        }
        else return;
        parsedTree->appendChild(new Node(Token::Star));
    }

    void Parser::parseString() {
        if (parsedTree->getToken() == Token::Import) {
            if (parsedTree->getLastChild()->getToken() != Token::Space)
                throw MalformedExpression("A space is needed between '@import' and the file name");
            removeSpace();
            parsedTree->setValue(currentToken->getValue());
        }
        else parseValue();
    }

    void Parser::parseGreatherThan() {
        Node *lastChild;
        Node *lastChildCopy = nullptr;
        Token token = parsedTree->getToken();
        if (token == Token::NullRoot || token == Token::BlockDefinition) {
            lastChild = parsedTree->getLastChild();
            if (lastChild != nullptr) {
                if (isWhiteSpace(lastChild->getToken())) {
                    removeWhiteSpaces();
                    lastChild = nullptr;
                }
                else if (lastChild->getToken() == Token::Name) lastChildCopy = new Node{Token::ElementName, lastChild->getValue()};
                else if (lastChild->getToken() == Token::AnyParent)
                    ; // do nothing, just ensure the node is being removed without being copied before
                else lastChildCopy = lastChild->copyNodeWithChilds();
                parsedTree->deleteSpecificChild(lastChild);
            }
            else {
                if (token == Token::NullRoot)
                    throw MissingToken("A '>' token must not be the first token of a block declaration if not a nested block");
            }
            parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock));
            parsedTree = parsedTree->appendChild(new Node(Token::BlockDeclaration));
            parsedTree = parsedTree->appendChild(new Node(Token::Declaration));
            parsedTree->appendChild(lastChildCopy);
            parsedTree->appendChild(new Node(Token::DirectParent));
        }
        else if (token == Token::Declaration) {
            lastChild = parsedTree->getLastChild();
            if (lastChild->getToken() == Token::AnyParent) {
                parsedTree->deleteSpecificChild(lastChild);
            }
            parsedTree->appendChild(new Node(Token::DirectParent));
        }
        else
            throw MalformedExpression(
                "A direct parent relation ('>') must be before a style block opening and at the root level of the style file or "
                "inside an other style block");
    }

    void Parser::parseOpeningParenthesis() {
        removeSpace();

        Node *lastChild;
        if (parsedTree->getToken() == Token::Assignment) {
            if (parsedTree->getNbChilds() > 1) {
                lastChild = parsedTree->getLastChild();
                if (lastChild != nullptr && lastChild->getToken() == Token::Name) {
                    parsedTree->replaceChild(lastChild, new Node{Token::Function, lastChild->getValue()});
                }
                else throw MalformedExpression("A tuple must be the only right value of an assignment");
            }
            else parsedTree = parsedTree->appendChild(new Node(Token::Tuple));
            return;
        }
        if (parsedTree->getToken() != Token::Tuple && parsedTree->getToken() != Token::Function)
            throw MalformedExpression("A tuple|function must follow an assignment symbol or be a function parameter or inside of a tuple");
        lastChild = parsedTree->getLastChild();
        if (lastChild != nullptr) {
            if (lastChild->getToken() == Token::Name) {
                lastChild->setToken(Token::Function);
                parsedTree = lastChild;
                return;
            }
            else if (lastChild->getToken() != Token::ArgSeparator)
                throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
        }
        parsedTree->deleteSpecificChild(lastChild);
        parsedTree = parsedTree->appendChild(new Node(Token::Tuple));
    }

    void Parser::parseClosingParenthesis() {
        removeSpace();

        if (parsedTree->getToken() != Token::Function && parsedTree->getToken() != Token::Tuple)
            throw MissingToken("A closing parenthesis ')' needs an opening parenthesis '('");
        parsedTree = parsedTree->getParent();
    }

    void Parser::parseOpeningCurlyBracket() {
        Node *lastChild;
        Node *lastChildCopy;
        removeSpace();
        lastChild = parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->getToken() == Token::AnyParent) {
            parsedTree->deleteSpecificChild(lastChild);
        }
        if (parsedTree->getToken() != Token::NullRoot && parsedTree->getToken() != Token::BlockDefinition
            && parsedTree->getToken() != Token::Declaration)
            throw MalformedExpression("A style block must be defined in an other style block or at the root level of the file");
        if (parsedTree->getToken() != Token::Declaration) {
            lastChild = parsedTree->getLastChild();
            if (lastChild == nullptr)
                throw MalformedExpression("A style block must start with at list an element name|class|identifier before the opening curly bracket");
            lastChildCopy = lastChild->copyNodeWithChilds();
            if (lastChildCopy->getToken() == Token::Name) lastChildCopy->setToken(Token::ElementName);
            parsedTree->deleteSpecificChild(lastChild);
            parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock));
            parsedTree = parsedTree->appendChild(new Node(Token::BlockDeclaration));
            parsedTree = parsedTree->appendChild(new Node(Token::Declaration));
            parsedTree->appendChild(lastChildCopy);
        }
        parsedTree = parsedTree->getParent()->getParent()->appendChild(new Node(Token::BlockDefinition));
    }

    void Parser::parseClosingCurlyBracket() {
        removeWhiteSpaces();

        if (parsedTree->getToken() == Token::Assignment) throw MissingToken("Missing semi-colon after assignment");
        else if (parsedTree->getToken() != Token::BlockDefinition)
            throw MissingToken("A closing curly bracket '}' needs an opening curly bracket '{'");
        parsedTree = parsedTree->getParent()->getParent();
    }

    void Parser::parsePseudoName() {
        Node *lastChild;
        if (parsedTree->getToken() == Token::Assignment) {
            removeSpace();

            if (parsedTree->getNbChilds() != 1) throw MalformedExpression("Can't have more than one rvalue in an assignment");
            parsedTree->appendChild(new Node{Token::NameString, currentToken->getValue()});
        }
        else if (parsedTree->getToken() == Token::Tuple || parsedTree->getToken() == Token::Function) {
            removeSpace();

            if (parsedTree->getNbChilds() > 1) throw MalformedExpression("Can only have one rvalue in an assignment");
            lastChild = parsedTree->getLastChild();
            if (lastChild != nullptr && lastChild->getToken() != Token::ArgSeparator)
                throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
            parsedTree->deleteSpecificChild(lastChild);
            parsedTree->appendChild(new Node{Token::PseudoName, parsedTree->getValue()});
        }
        else {
            if (isValidElementOrRuleName(currentToken->getValue())) parseName();
            else throw MalformedExpression("Illegal pseudo name placement");
        }
    }

    void Parser::parseName() {
        Token token = parsedTree->getToken();
        Node *lastChild;
        Node *lastChildCopy;
        if (token == Token::NullRoot) {
            removeWhiteSpaces();

            parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock));
            parsedTree = parsedTree->appendChild(new Node(Token::BlockDeclaration));
            parsedTree = parsedTree->appendChild(new Node(Token::Declaration));
            parsedTree->appendChild(new Node{Token::ElementName, currentToken->getValue()});
            return;
        }
        if (token == Token::BlockDefinition) {
            removeWhiteSpaces();
            lastChild = parsedTree->getLastChild();

            if (lastChild != nullptr && lastChild->getToken() == Token::PseudoName) {
                lastChildCopy = lastChild->copyNodeWithChilds();
                parsedTree->deleteSpecificChild(lastChild);
                parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock));
                parsedTree = parsedTree->appendChild(new Node(Token::BlockDeclaration));
                parsedTree = parsedTree->appendChild(new Node(Token::Declaration));
                parsedTree->appendChild(lastChildCopy);
                parsedTree->appendChild(new Node{Token::ElementName, currentToken->getValue()});
            }
            else parsedTree->appendChild(new Node{Token::Name, currentToken->getValue()});
            return;
        }

        if (token == Token::Assignment) {
            removeSpace();

            if (parsedTree->getNbChilds() > 1) throw MalformedExpression("A string|function must be the only right value of an assignment");
            parsedTree->appendChild(new Node{Token::Name, currentToken->getValue()});
            return;
        }
        if (token == Token::Declaration) {
            parsedTree->appendChild(new Node{Token::ElementName, currentToken->getValue()});
            return;
        }
        removeSpace();

        if (token != Token::Tuple && token != Token::Function)
            throw MalformedExpression("A string|function must follow an assignment symbol or be inside of a tuple or a function parameter");
        lastChild = parsedTree->getLastChild();
        if (lastChild != nullptr && lastChild->getToken() != Token::ArgSeparator)
            throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
        parsedTree->deleteSpecificChild(lastChild);
        parsedTree->appendChild(currentToken->copyNode());
    }

    void Parser::parseUnit() {
        Node *lastChild;
        Node *newChild;
        if (parsedTree->getToken() != Token::Assignment && parsedTree->getToken() != Token::Function && parsedTree->getToken() != Token::Tuple)
            throw MalformedExpression("An unit must be inside an assignment, a function or a tuple");

        lastChild = parsedTree->getLastChild();
        if (lastChild == nullptr || (lastChild->getToken() != Token::Int && lastChild->getToken() != Token::Float))
            throw MissingToken("A unit must have an int or a float before");
        newChild = new Node{currentToken->getToken(), currentToken->getValue()};
        newChild->appendChild(lastChild->copyNodeWithChilds());
        parsedTree->replaceChild(lastChild, newChild);
    }

    Node *Parser::updateLastDeclarationComponentBeforeNewOne(Node *lastChild) {
        Node *finalChild = nullptr;
        if (lastChild != nullptr) {
            if (isWhiteSpace(lastChild->getToken())) {
                removeWhiteSpaces();
                lastChild = nullptr;
            }
            else if (lastChild->getToken() == Token::Name) finalChild = new Node{Token::ElementName, lastChild->getValue()};
            else finalChild = lastChild->copyNodeWithChilds();
            parsedTree->deleteSpecificChild(lastChild);
        }
        return finalChild;
    }

    void Parser::parseDeclarationComponent(Token outputTokenType) {
        Node *lastChild;
        Node *lastChildCopy = nullptr;
        Token token = parsedTree->getToken();
        if (token == Token::NullRoot || token == Token::BlockDefinition) {
            lastChild = parsedTree->getLastChild();
            lastChildCopy = updateLastDeclarationComponentBeforeNewOne(lastChild);
            parsedTree = parsedTree->appendChild(new Node(Token::StyleBlock));
            parsedTree = parsedTree->appendChild(new Node(Token::BlockDeclaration));
            parsedTree = parsedTree->appendChild(new Node(Token::Declaration));
            parsedTree->appendChild(lastChildCopy);
            parsedTree->appendChild(new Node{outputTokenType, currentToken->getValue()});
        }
        else if (token == Token::Declaration) {
            parsedTree->appendChild(new Node{outputTokenType, currentToken->getValue()});
        }
        else
            throw MalformedExpression(
                "A " + tokenToString(outputTokenType)
                + " must be before a style block opening and at the root level of the style file or inside an other style block");
    }

    void Parser::parseClass() { parseDeclarationComponent(Token::Class); }

    void Parser::parseIdentifier() { parseDeclarationComponent(Token::Identifier); }

    void Parser::parseModifier() { parseDeclarationComponent(Token::Modifier); }

} // namespace style
