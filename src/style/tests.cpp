#include <string>
#include <iostream>

#include "node.hpp"
#include "parser.hpp"
#include "solver.hpp"
#include "tokenizer.hpp"
#include "number.hpp"

using namespace std;

void testExpressionsStrings(bool equal, const string &expr, const string &expected, Settings *settings) {
    cerr << "Test tokenizer + parser " << expr << " == \"" << expected << "\" : ";
    try {
        Node *tokens = Tokenizer(expr, settings).getResult();
        Node *result = Parser(tokens, settings).getFinalTree();
        if ((result->str() == expected) == equal) cerr << "OK";
        else cerr << "KO";
        delete result;
        delete tokens;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testExpressions(bool equal, const string &expr, const string &expected, Settings *settings) {
    cerr << "Test expr " << expr << " == " << expected << " : ";
    try {
        Node *tokens = Tokenizer(expr, settings).getResult();
        Node *result = Parser(tokens, settings).getFinalTree();
        Node *tokens2 = Tokenizer(expected, settings).getResult();
        Node *result2 = Parser(tokens2, settings).getFinalTree();
        if ((result->str() == result2->str()) == equal) cerr << "OK";
        else cerr << "KO";
        delete result;
        delete result2;
        delete tokens;
        delete tokens2;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testTokenizer(bool equal, const string &expr, const Node *expected, Settings *settings) {
    cerr << "Test tokenizer " << expr << " == " << endl;
    cerr << " : ";
    try {
        Node *result = Tokenizer(expr, settings).getResult();
        Node *n = result;
        while (n != nullptr) {
            if ((expected == nullptr || !(*n == *expected)) == equal) {
                cerr << "KO" << endl;
                delete result;
                return;
            }
            n = n->getNext();
            expected = expected->getNext();
        }
        if (expected != nullptr) cerr << "KO";
        else cerr << "OK";
        delete result;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testParser(bool equal, Node *expr, const Node *expected, Settings *settings) {
    cerr << "Test parser " << *expr << " == " << *expected << " : ";
    try {
        Node *result = Parser(expr, settings).getFinalTree();
        if (areSameNodes(result, expected) == equal) cerr << "OK";
        else cerr << "KO";
        delete result;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}

void testTokenizerAndParser(bool equal, const string &expr, const Node *expected, Settings *settings) {
    cerr << "Test tokenizer + parser " << expr << " == " << *expected << " : ";
    try {
        Node *tokens = Tokenizer(expr, settings).getResult();
        Node *result = Parser(tokens, settings).getFinalTree();
        if (areSameNodes(result, expected) == equal) cerr << "OK";
        else cerr << "KO";
        delete result;
        delete tokens;
    }
    catch (const exception& e) {
        cerr << "Failed with error : " << e.what();
    }
    cerr << endl;
}

string testBools(bool equal, bool a, bool b) {
    if ((a == b) == equal) return "OK";
    return "Error, " + to_string(a) + " instead of " + to_string(b);
}

string testStrings(bool equal, const string &a, const string &b) {
    if ((a == b) == equal) return "OK";
    return "Error, " + a + " instead of " + b;
}

void isGreaterNumber(Number *a, Number *b, bool expected) {
    cerr << *a << " > " << *b << " : " << testBools(true, a->isGreaterThan(b), expected) << endl;
}

void isLowerNumber(Number *a, Number *b, bool expected) {
    cerr << *a << " < " << *b << " : " << testBools(true, a->isLowerThan(b), expected) << endl;
}

void isEqualNumber(Number *a, Number *b, bool expected) {
    cerr << *a << " == " << *b << " : " << testBools(true, a->isEqualTo(b), expected) << endl;
}

void testInvalidExpression(string expression, Settings* settings) {
    cerr << "Test InvalidExpression exception with expression '" << expression << "' : ";
    Node *tokens = nullptr;
    Node *result = nullptr;
    try {
        tokens = Tokenizer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        delete result;
        delete tokens;
        cerr << "KO";
    }
    catch (const InvalidExpression &e) {
        cerr << "OK";
    }
    catch (const exception &e) {
        cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    cerr << endl;
}

void testUnknownToken(string expression, Settings* settings) {
    cerr << "Test UnknownToken exception with expression '" << expression << "' : ";
    Node *tokens = nullptr;
    Node *result = nullptr;
    try {
        tokens = Tokenizer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        delete result;
        delete tokens;
        cerr << "KO";
    }
    catch (const UnknownToken &e) {
        cerr << "OK";
    }
    catch (const exception &e) {
        cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    cerr << endl;
}

void testMissingToken(string expression, Settings* settings) {
    cerr << "Test MissingToken exception with expression '" << expression << "' : ";
    Node *tokens = nullptr;
    Node *result = nullptr;
    try {
        tokens = Tokenizer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        delete result;
        delete tokens;
        cerr << "KO";
    }
    catch (const MissingToken &e) {
        cerr << "OK";
    }
    catch (const exception &e) {
        cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    cerr << endl;
}

void testUnknownValue(string expression, Settings* settings) {
    cerr << "Test UnknownValue exception with expression '" << expression << "' : ";
    Node *tokens = nullptr;
    Node *result = nullptr;
    try {
        tokens = Tokenizer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        delete result;
        delete tokens;
        cerr << "KO";
    }
    catch (const UnknownValue &e) {
        cerr << "OK";
    }
    catch (const exception &e) {
        cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    cerr << endl;
}

int main() {
    Settings *settings = new Settings;
    settings->debug = false;
    testExpressions(true, "3x**2", "3x^2", settings);
    testExpressions(false, "3x**2", "3x*2", settings);
    testExpressionsStrings(true, "3x**2", "3*x^2", settings);
    testExpressionsStrings(false, "3x**2", "3x^2", settings);
    settings->implicitMultiplicationPriority = true;
    testExpressionsStrings(true, "3x**2*(-2)+3", "3*x^2*(0-2)+3", settings);
    settings->implicitMultiplicationPriority = false;
    Node *value = new Node{Token::Minus};
    Node *next1 = new Number{"3"};
    Node *next2 = new Node{Token::Name, "x"};
    value->appendNext(next1);
    next1->appendNext(next2);
    testTokenizer(true, "-3x", value, settings);
    delete value;
    Number *a = new Number{"53.9"};
    Number *a2 = new Number{"53.9"};
    Number *b = new Number{"-92.5"};
    Number *c = new Number{"-3.2"};
    Number *one = new Number{"1"};
    Number *zero = new Number{"0"};
    Number *zeroFloat = new Number{"0.0"};
    isGreaterNumber(a, b, true);
    isLowerNumber(b, a, true);
    isLowerNumber(a, b, false);
    isGreaterNumber(b, a, false);
    isGreaterNumber(a, a, false);
    isLowerNumber(a, a, false);
    isEqualNumber(a, a, true);
    isEqualNumber(b, b, true);
    isEqualNumber(a, b, false);
    isEqualNumber(b, a, false);
    cerr << *a << " + " << *b << " : ";
    a->add(b);
    cerr << testStrings(true, a->getValue(), "-38.6") << endl;
    cerr << *b << " + " << *a2 << " : ";
    b->add(a2);
    cerr << testStrings(true, b->getValue(), "-38.6") << endl;
    cerr << *a2 << " + " << *a2 << " : ";
    a2->add(a2);
    cerr << testStrings(true, a2->getValue(), "107.8") << endl;
    cerr << *c << " + " << *c << " : ";
    c->add(c);
    cerr << testStrings(true, c->getValue(), "-6.4") << endl;
    isEqualNumber(zero, zeroFloat, true);
    isGreaterNumber(zero, a, true);
    isLowerNumber(zero, c, false);
    cerr << *zero << " + " << *one << " : ";
    zero->add(one);
    cerr << testStrings(true, zero->getValue(), "1") << endl;
    cerr << *zeroFloat << " + " << *one << " : ";
    zeroFloat->add(one);
    cerr << testStrings(true, zeroFloat->getValue(), "1") << endl;
    cerr << *one << " + " << *one << " : ";
    one->add(one);
    cerr << testStrings(true, one->getValue(), "2") << endl;
    delete a;
    delete a2;
    delete b;
    delete c;
    delete one;
    delete zero;
    delete zeroFloat;
    testInvalidExpression("a,b", settings);
    testInvalidExpression("a+*b", settings);
    testInvalidExpression("a()", settings);
    testInvalidExpression("()", settings);
    testMissingToken("a(", settings);
    testMissingToken("a)", settings);
    testUnknownValue("a~b", settings);
    testUnknownToken("a!", settings);
    delete settings;
    return 0;
}
