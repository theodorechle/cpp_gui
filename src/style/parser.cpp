using namespace std;

#include "parser.hpp"

void Parser::goToParentBlock() {
    if (isNodeNull(expressionTree)
        || isNodeNull(expressionTree->getParent())
        || expressionTree->getParent()->getTokenType() == Token::StyleBlock
        || expressionTree->getParent()->getTokenType() == Token::Function
        || expressionTree->getParent()->getTokenType() == Token::Tuple
        ) return;
    expressionTree = expressionTree->getParent();
    goToParentBlock();
}

void Parser::parse() {
    try {
        while (expressionTokens != nullptr) {
            switch (expressionTokens->getTokenType())
            {
            case Token::Space:
                parseSpace();
                break;
            case Token::LineReturn:
                parseLineReturn();
                break;
            case Token::Int:
            case Token::Float:
            case Token::Bool:
            case Token::String:
                parseValue();
            case Token::Comma:
                parseComma();
                break;
            case Token::SemiColon:
                parseSemiColon();
                break;
            case Token::Unit:
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
            case Token::Class:
                parseClass();
                break;
            case Token::Modifier:
                parseModifier();
                break;
            case Token::Identifier:
                parseIdentifier();
                break;
            case Token::Name:
                parseName();
                break;
            default:
                throw UnknownToken(*expressionTokens);
                break;
            }
            if (settings->debug) {
                cerr << endl << "Root : " << endl;
                root(expressionTree)->display(cerr);
                cerr << endl;
            }
            expressionTokens = expressionTokens->getNext();
        }
    }
    catch (const exception &) {
        expressionTree = nullptr;
        delete expressionTreeRoot;
        throw;
    }
    if (expressionTree == expressionTreeRoot) {
        expressionTree = nullptr;
        delete expressionTreeRoot;
        expressionTreeRoot = nullptr;
        return;
    }
    expressionTreeRoot = expressionTreeRoot->getChild();
    deleteNullRoot(expressionTreeRoot);
}

void Parser::parseSpace() {
    expressionTree->appendChild(expressionTokens->copyNode());
}

void Parser::parseLineReturn() {
    expressionTree->appendChild(expressionTokens->copyNode());
}

void Parser::parseValue() {
    Node* lastChild;
    if (expressionTree->getTokenType() != Token::Assignment) {
        if (expressionTree->getNbChilds() > 1) throw MalformedExpression("Can only have one rvalue in an assignment");
        if (expressionTree->getTokenType() != Token::Tuple && expressionTree->getTokenType() != Token::Function) throw MalformedExpression("An int|float|bool|string must follow an assignment symbol or be inside of a tuple or a function parameter");
        lastChild = expressionTree->getLastChild();
        if (lastChild != nullptr && lastChild->getTokenType() != Token::ArgSeparator) throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
        expressionTree->removeSpecificChild(lastChild);
        expressionTree->appendChild(expressionTree->copyNode());
    }
    expressionTree->appendChild(expressionTokens->copyNode());
}

void Parser::parseComma() {
    if (expressionTree->getTokenType() != Token::Tuple) throw MalformedExpression("A comma must be inside of a tuple");
    expressionTree->appendChild(new Node{Token::ArgSeparator});
}

void Parser::parseColon() {
    if (expressionTree->getTokenType() != Token::StyleDefinitions) throw MalformedExpression("A colon must be inside of a style block");
    Node* lastChild = expressionTree->getLastChild();
    if (lastChild == nullptr) throw MissingToken("A colon must follow a style name");
    Node* newChild = new Node{Token::Assignment};
    newChild->appendChild(lastChild->copyNode());
    expressionTree->replaceChild(lastChild, newChild);
    expressionTree = newChild;
}

void Parser::parseSemiColon() {
    if (expressionTree->getTokenType() != Token::Assignment) throw MalformedExpression("A semi-colon must be at the end of an assignment");
    expressionTree = expressionTree->getParent();
}

void Parser::parseName() {
    Token token = expressionTree->getTokenType();
    if (token == Token::NullRoot || token == Token::StyleDefinitions) {
        expressionTree = expressionTree->appendChild(new Node{Token::StyleDefinitions});
        expressionTree = expressionTree->appendChild(new Node{Token::BlockPrototype});
        expressionTree->appendChild(new Node{Token::ElementName, expressionTokens->getValue()});
        return;
    }
    if (token == Token::StyleDefinitions) {
        expressionTree = expressionTree->appendChild(new Node{Token::StyleName, expressionTokens->getValue()});
        return;
    }
    if (token == Token::Assignment) {
        if (expressionTree->getNbChilds() > 1) throw MalformedExpression("A function or a string must be the only right value of an assignment");
        expressionTree->appendChild(new Node{Token::Name, expressionTokens->getValue()});
        return;
    }
    if (token != Token::Tuple && token != Token::Function) throw MalformedExpression("A string|function must follow an assignment symbol or be inside of a tuple or a function parameter");
    Node* lastChild = expressionTree->getLastChild();
    if (lastChild != nullptr && lastChild->getTokenType() != Token::ArgSeparator) throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
    expressionTree->removeSpecificChild(lastChild);
    expressionTree->appendChild(expressionTree->copyNode());
}

void Parser::parseOpeningParenthesis() {
    Node* lastChild;
    if (expressionTree->getTokenType() == Token::Assignment) {
        if (expressionTree->getNbChilds() > 1) {
            lastChild = expressionTree->getLastChild();
            if (lastChild->getTokenType() == Token::Name) {
                expressionTree->replaceChild(lastChild, new Node{Token::Function, lastChild->getValue()});
            }
            else throw MalformedExpression("A tuple must be the only right value of an assignment");
        }
        else expressionTree->appendChild(new Node{Token::Tuple});
        return;
    }
    if (expressionTree->getTokenType() != Token::Tuple && expressionTree->getTokenType() != Token::Function) throw MalformedExpression("A tuple|function must follow an assignment symbol or be a function parameter or inside of a tuple");
    lastChild = expressionTree->getLastChild();
    if (lastChild->getTokenType() == Token::Name) {
        expressionTree->replaceChild(lastChild, new Node{Token::Function, lastChild->getValue()});
    }
    else if (lastChild != nullptr && lastChild->getTokenType() != Token::ArgSeparator) throw MalformedExpression("The elements in a tuple or the parameters of a function must be comma separated");
    expressionTree->removeSpecificChild(lastChild);
    expressionTree->appendChild(new Node{Token::Tuple});
}

void Parser::parseClosingParenthesis() {
    goToParentBlock();
    if (expressionTree->getTokenType() != Token::Function
        && expressionTree->getTokenType() != Token::Tuple) throw MissingToken("A closing parenthesis ')' need an opening parenthesis '('");
}