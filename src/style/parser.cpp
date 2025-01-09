#include "parser.hpp"

using namespace std;

bool Parser::isValidName(const string &str, size_t start, size_t end) {
    size_t i;
    end = min(end, str.size());
    for (i = start; i < end - 1; i++) {
        if (!isalpha(str[i]) && str[i] != '_' && str[i] != '-') return false;
    }
    if (!isalpha(str[i]) && str[i] != '_') return false;
    return true;
}

bool Parser::isValidElementOrStyleName(const string &str) {
    if (!isalpha(str[0])) return false;
    return isValidName(str, 1, str.size());
}

bool Parser::isValidClass(const string &str) {
    if (str.size() == 1 || str[0] != '.') return false;
    return isValidName(str, 1, str.size());
}

bool Parser::isValidIdentifier(const string &str) {
    if (str.size() == 1 || str[0] != '#') return false;
    return isValidName(str, 1, str.size());
}

bool Parser::isValidModifier(const string &str) {
    if (str.size() == 1 || str[0] != ':') return false;
    return isValidName(str, 1, str.size());
}

bool Parser::isValidHex(const std::string &str) {
    if (str[0] != '#') return false;
    return std::all_of(++str.cbegin(), str.cend(), ::isxdigit);
}

bool Parser::isWhiteSpace(Token token) {
    return (token == Token::Space || token == Token::LineReturn);
}

bool Parser::isComponentRelation(Token token) {
    return (token == Token::AnyParent || token == Token::DirectParent);
}

void Parser::removeSpace() {
    Node *lastChild = expressionTree->getLastChild();
    if (lastChild != nullptr && lastChild->getTokenType() == Token::Space) expressionTree->removeSpecificChild(lastChild);
}

void Parser::removeLineReturn() {
    Node *lastChild = expressionTree->getLastChild();
    if (lastChild != nullptr && lastChild->getTokenType() == Token::LineReturn) expressionTree->removeSpecificChild(lastChild);
}

void Parser::removeWhiteSpaces() {
    Node *lastChild = expressionTree->getLastChild();
    while (lastChild != nullptr && (lastChild->getTokenType() == Token::Space || lastChild->getTokenType() == Token::LineReturn)) {
        expressionTree->removeSpecificChild(lastChild);
        lastChild = expressionTree->getLastChild();
    }
}

void Parser::parse() {
    try {
        while (expressionTokens != nullptr) {
            if (settings->debug) cerr << "\nActual token : " << tokenToString(expressionTokens->getTokenType()) << ": '" << expressionTokens->getValue() << "'" << "\n";
            switch (expressionTokens->getTokenType()) {
            case Token::Space:
                parseSpace();
                break;
            case Token::LineReturn:
                parseLineReturn();
                break;
            case Token::OneLineComment:
                parseOneLineComment();
                break;
            case Token::MultiLineComment:
                parseMultiLineComment();
                break;
            case Token::Int:
            case Token::Float:
            case Token::Bool:
                parseValue();
                break;
            case Token::String:
                parseString();
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
                throw UnknownToken(*expressionTokens);
                break;
            }
            if (settings->debug) {
                cerr << "Root :\n";
                expressionTreeRoot->display(cerr);
                cerr << "\n";
            }
            expressionTokens = expressionTokens->getNext();
        }
        removeWhiteSpaces();
        if (settings->debug) {
            cerr << "Final parsed tree :\n";
            expressionTreeRoot->display(cerr);
            cerr << "\n";
        }
    }
    catch (const ParserError &) {
        expressionTree = nullptr;
        delete expressionTreeRoot;
        throw;
    }
}

void Parser::parseSpace() {
    Node *lastChild;
    if (expressionTree->getTokenType() == Token::Declaration) {
        if (expressionTree->getNbChilds() > 0) {
            lastChild = expressionTree->getLastChild();
            if (lastChild == nullptr || !isComponentRelation(lastChild->getTokenType())) expressionTree->appendChild(new Node{Token::AnyParent});
        }
    }
    else expressionTree->appendChild(expressionTokens->copyNode());
}

void Parser::parseLineReturn() {
    Token token = expressionTree->getTokenType();
    if (token != Token::NullRoot && token != Token::BlockDefinition) {
        throw MalformedExpression("A line return can only be between blocks declarations and between assignments");
    }
    expressionTree->appendChild(expressionTokens->copyNode());
}

void Parser::parseOneLineComment() {
}

void Parser::parseMultiLineComment() {
}

void Parser::parseValue() {
    removeSpace();

    Node *lastChild;
    if (expressionTree->getTokenType() != Token::Assignment) {
        if (expressionTree->getTokenType() != Token::Tuple && expressionTree->getTokenType() != Token::Function) throw MalformedExpression("An int|float|bool|string must follow an assignment symbol or be inside of a tuple or a function parameter");
        lastChild = expressionTree->getLastChild();
        if (lastChild != nullptr && lastChild->getTokenType() != Token::ArgSeparator) throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
        expressionTree->removeSpecificChild(lastChild);
        expressionTree->appendChild(expressionTokens->copyNode());
    }
    else {
        if (expressionTree->getNbChilds() > 1) throw MalformedExpression("Can only have one rvalue in an assignment");
        expressionTree->appendChild(expressionTokens->copyNode());
    }
}

void Parser::parseComma() {
    removeSpace();
    switch (expressionTree->getTokenType()) {
    case Token::Tuple:
        expressionTree->appendChild(new Node{Token::ArgSeparator});
        break;
    case Token::Declaration:
        expressionTree = expressionTree->getParent()->appendChild(new Node{Token::Declaration});
        break;
    default:
        throw MalformedExpression("A comma must be inside of a tuple");
    }
}

void Parser::parseColon() {
    removeSpace();

    Node *lastChild;
    Node *newChild;
    if (expressionTree->getTokenType() != Token::BlockDefinition) throw MalformedExpression("A colon must be inside of a style block");
    lastChild = expressionTree->getLastChild();
    if (lastChild == nullptr) throw MissingToken("A colon must follow a style name");
    lastChild->setTokenType(Token::StyleName); // should be Token::Name before
    newChild = new Node{Token::Assignment};
    newChild->appendChild(lastChild->copyNodeWithChilds());
    expressionTree->replaceChild(lastChild, newChild);
    expressionTree = newChild;
}

void Parser::parseSemiColon() {
    removeSpace();

    if (expressionTree->getTokenType() != Token::Assignment) throw MalformedExpression("A semi-colon must be at the end of an assignment");
    expressionTree = expressionTree->getParent();
}

void Parser::parseGreatherThan() {
    Node *lastChild;
    Node *lastChildCopy = nullptr;
    Token token = expressionTree->getTokenType();
    if (token == Token::NullRoot || token == Token::BlockDefinition) {
        lastChild = expressionTree->getLastChild();
        if (lastChild != nullptr) {
            if (isWhiteSpace(lastChild->getTokenType())) removeWhiteSpaces();
            else if (lastChild->getTokenType() == Token::Name) lastChildCopy = new Node{Token::ElementName, lastChild->getValue()};
            else if (lastChild->getTokenType() == Token::AnyParent); // do nothing, just ensure the node is being removed without being copied before
            else lastChildCopy = lastChild->copyNodeWithChilds();
            expressionTree->removeSpecificChild(lastChild);
        }
        else {
            if (token == Token::NullRoot) throw MissingToken("A '>' token must not be the first token of a block declaration if not a nested block");
        }
        expressionTree = expressionTree->appendChild(new Node{Token::StyleBlock});
        expressionTree = expressionTree->appendChild(new Node{Token::BlockDeclaration});
        expressionTree = expressionTree->appendChild(new Node{Token::Declaration});
        expressionTree->appendChild(lastChildCopy);
        expressionTree->appendChild(new Node{Token::DirectParent});
    }
    else if (token == Token::Declaration) {
        lastChild = expressionTree->getLastChild();
        if (lastChild->getTokenType() == Token::AnyParent) {
            expressionTree->removeSpecificChild(lastChild);
        }
        expressionTree->appendChild(new Node{Token::DirectParent});
    }
    else throw MalformedExpression("A direct parent relation must be before a style block opening and at the root level of the style file or inside an other style block");
}

void Parser::parseOpeningParenthesis() {
    removeSpace();

    Node *lastChild;
    if (expressionTree->getTokenType() == Token::Assignment) {
        if (expressionTree->getNbChilds() > 1) {
            lastChild = expressionTree->getLastChild();
            if (lastChild != nullptr && lastChild->getTokenType() == Token::Name) {
                expressionTree->replaceChild(lastChild, new Node{Token::Function, lastChild->getValue()});
            }
            else throw MalformedExpression("A tuple must be the only right value of an assignment");
        }
        else expressionTree = expressionTree->appendChild(new Node{Token::Tuple});
        return;
    }
    if (expressionTree->getTokenType() != Token::Tuple && expressionTree->getTokenType() != Token::Function) throw MalformedExpression("A tuple|function must follow an assignment symbol or be a function parameter or inside of a tuple");
    lastChild = expressionTree->getLastChild();
    if (lastChild != nullptr) {
        if (lastChild->getTokenType() == Token::Name) {
            lastChild->setTokenType(Token::Function);
            expressionTree = lastChild;
            return;
        }
        else if (lastChild->getTokenType() != Token::ArgSeparator) throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
    }
    expressionTree->removeSpecificChild(lastChild);
    expressionTree = expressionTree->appendChild(new Node{Token::Tuple});
}

void Parser::parseClosingParenthesis() {
    removeSpace();

    if (expressionTree->getTokenType() != Token::Function && expressionTree->getTokenType() != Token::Tuple) throw MissingToken("A closing parenthesis ')' needs an opening parenthesis '('");
    expressionTree = expressionTree->getParent();
}

void Parser::parseOpeningCurlyBracket() {
    Node *lastChild;
    Node *lastChildCopy;
    removeSpace();
    lastChild = expressionTree->getLastChild();
    if (lastChild != nullptr && lastChild->getTokenType() == Token::AnyParent) {
        expressionTree->removeSpecificChild(lastChild);
    }
    if (expressionTree->getTokenType() != Token::NullRoot && expressionTree->getTokenType() != Token::BlockDefinition && expressionTree->getTokenType() != Token::Declaration) throw MalformedExpression("A style block must be defined in an other style block or at the root level of the file");
    if (expressionTree->getTokenType() != Token::Declaration) {
        lastChild = expressionTree->getLastChild();
        if (lastChild == nullptr) throw MalformedExpression("A style block must start with at list an element name|class|identifier before the opening curly bracket");
        lastChildCopy = lastChild->copyNodeWithChilds();
        if (lastChildCopy->getTokenType() == Token::Name) lastChildCopy->setTokenType(Token::ElementName);
        expressionTree->removeSpecificChild(lastChild);
        expressionTree = expressionTree->appendChild(new Node{Token::StyleBlock});
        expressionTree = expressionTree->appendChild(new Node{Token::BlockDeclaration});
        expressionTree = expressionTree->appendChild(new Node{Token::Declaration});
        expressionTree->appendChild(lastChildCopy);
    }
    expressionTree = expressionTree->getParent()->getParent()->appendChild(new Node{Token::BlockDefinition});
}

void Parser::parseClosingCurlyBracket() {
    removeWhiteSpaces();

    if (expressionTree->getTokenType() != Token::BlockDefinition) throw MalformedExpression("A closing curly bracket '}' needs an opening curly bracket '{'");
    expressionTree = expressionTree->getParent()->getParent();
}

void Parser::parseUnit() {
    Node *lastChild;
    Node *newChild;
    if (expressionTree->getTokenType() != Token::Assignment && expressionTree->getTokenType() != Token::Function && expressionTree->getTokenType() != Token::Tuple) throw MalformedExpression("An unit must be inside an assignment, a function or a tuple");

    lastChild = expressionTree->getLastChild();
    if (lastChild == nullptr || (lastChild->getTokenType() != Token::Int && lastChild->getTokenType() != Token::Float)) throw MissingToken("A unit must have an int or a float before");
    newChild = new Node{expressionTokens->getTokenType(), expressionTokens->getValue()};
    newChild->appendChild(lastChild->copyNodeWithChilds());
    expressionTree->replaceChild(lastChild, newChild);
}

void Parser::parseString() {
    Node *lastChild;
    if (expressionTree->getTokenType() == Token::Assignment) {
        removeSpace();

        if (expressionTree->getNbChilds() != 1) throw MalformedExpression("Can't have more than one rvalue in an assignment");
        if (isValidHex(expressionTokens->getValue())) expressionTree->appendChild(new Node{Token::Hex, expressionTokens->getValue().substr(1)});
        else expressionTree->appendChild(new Node{Token::String, expressionTokens->getValue()});
    }
    else if (expressionTree->getTokenType() == Token::Tuple || expressionTree->getTokenType() == Token::Function) {
        removeSpace();

        if (expressionTree->getNbChilds() > 1) throw MalformedExpression("Can only have one rvalue in an assignment");
        lastChild = expressionTree->getLastChild();
        if (lastChild != nullptr && lastChild->getTokenType() != Token::ArgSeparator) throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
        expressionTree->removeSpecificChild(lastChild);
        expressionTree->appendChild(new Node{Token::String, expressionTree->getValue()});
    }
    else {
        if (isValidClass(expressionTokens->getValue())) parseClass();
        else if (isValidIdentifier(expressionTokens->getValue())) parseIdentifier();
        else if (isValidModifier(expressionTokens->getValue())) parseModifier();
        else if (isValidElementOrStyleName(expressionTokens->getValue())) parseName();
        else throw MalformedExpression("Illegal string placement");
    }
}

void Parser::parseName() {
    Token token = expressionTree->getTokenType();
    Node *lastChild;
    Node *lastChildCopy;
    if (token == Token::NullRoot) {
        removeWhiteSpaces();

        expressionTree = expressionTree->appendChild(new Node{Token::StyleBlock});
        expressionTree = expressionTree->appendChild(new Node{Token::BlockDeclaration});
        expressionTree = expressionTree->appendChild(new Node{Token::Declaration});
        expressionTree->appendChild(new Node{Token::ElementName, expressionTokens->getValue()});
        return;
    }
    if (token == Token::BlockDefinition) {
        removeWhiteSpaces();
        lastChild = expressionTree->getLastChild();

        if (lastChild != nullptr && lastChild->getTokenType() == Token::String) {
            lastChildCopy = lastChild->copyNodeWithChilds();
            expressionTree->removeSpecificChild(lastChild);
            expressionTree = expressionTree->appendChild(new Node{Token::StyleBlock});
            expressionTree = expressionTree->appendChild(new Node{Token::BlockDeclaration});
            expressionTree = expressionTree->appendChild(new Node{Token::Declaration});
            expressionTree->appendChild(lastChildCopy);
            expressionTree->appendChild(new Node{Token::ElementName, expressionTokens->getValue()});
        }
        else expressionTree->appendChild(new Node{Token::Name, expressionTokens->getValue()});
        return;
    }

    if (token == Token::Assignment) {
        removeSpace();

        if (expressionTree->getNbChilds() > 1) throw MalformedExpression("A string|function must be the only right value of an assignment");
        expressionTree->appendChild(new Node{Token::Name, expressionTokens->getValue()});
        return;
    }
    if (token == Token::Declaration) {
        expressionTree->appendChild(new Node{Token::ElementName, expressionTokens->getValue()});
        return;
    }
    removeSpace();

    if (token != Token::Tuple && token != Token::Function) throw MalformedExpression("A string|function must follow an assignment symbol or be inside of a tuple or a function parameter");
    lastChild = expressionTree->getLastChild();
    if (lastChild != nullptr && lastChild->getTokenType() != Token::ArgSeparator) throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
    expressionTree->removeSpecificChild(lastChild);
    expressionTree->appendChild(expressionTokens->copyNode());
}

Node *Parser::updateLastDeclarationComponentBeforeNewOne(Node *lastChild) {
    Node *finalChild = nullptr;
    if (lastChild != nullptr) {
        if (isWhiteSpace(lastChild->getTokenType())) removeWhiteSpaces();
        else if (lastChild->getTokenType() == Token::Name) finalChild = new Node{Token::ElementName, lastChild->getValue()};
        else finalChild = lastChild->copyNodeWithChilds();
        expressionTree->removeSpecificChild(lastChild);
    }
    return finalChild;
}

void Parser::parseDeclarationComponent(Token outputTokenType) {
    Node *lastChild;
    Node *lastChildCopy = nullptr;
    Token token = expressionTree->getTokenType();
    if (token == Token::NullRoot || token == Token::BlockDefinition) {
        lastChild = expressionTree->getLastChild();
        lastChildCopy = updateLastDeclarationComponentBeforeNewOne(lastChild);
        expressionTree = expressionTree->appendChild(new Node{Token::StyleBlock});
        expressionTree = expressionTree->appendChild(new Node{Token::BlockDeclaration});
        expressionTree = expressionTree->appendChild(new Node{Token::Declaration});
        expressionTree->appendChild(lastChildCopy);
        expressionTree->appendChild(new Node{outputTokenType, expressionTokens->getValue().substr(1)});
    }
    else if (token == Token::Declaration) {
        expressionTree->appendChild(new Node{outputTokenType, expressionTokens->getValue().substr(1)});
    }
    else throw MalformedExpression("A " + tokenToString(outputTokenType) + "must be before a style block opening and at the root level of the style file or inside an other style block");
}

void Parser::parseClass() {
    parseDeclarationComponent(Token::Class);
}

void Parser::parseIdentifier() {
    parseDeclarationComponent(Token::Identifier);
}

void Parser::parseModifier() {
    parseDeclarationComponent(Token::Modifier);
}