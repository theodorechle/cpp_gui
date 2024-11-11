#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "../node.hpp"
#include "../tokenizer.hpp"
#include "../parser.hpp"
#include "../solver.hpp"

void testTokenizer(bool equal, const std::string &expr, const Node* expected, Settings* settings) {
    std::cerr << "Test if tokenizing\n'\n" << expr << "\n'\n";
    if (equal) std::cerr << "equals to\n";
    else std::cerr << "differs from\n";
    expected->displayNexts(std::cerr);
    std::cerr << ": ";
    try {
        Node* result = Tokenizer(expr, settings).getResult();
        Node* n = result;
        while (n != nullptr) {
            if ((expected == nullptr || !(*n == *expected)) == equal) {
                std::cerr << "KO" << std::endl;
                delete result;
                return;
            }
            n = n->getNext();
            expected = expected->getNext();
        }
        if (expected != nullptr) std::cerr << "KO";
        else std::cerr << "OK";
        delete result;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed with error : " << e.what();
    }
    std::cerr << std::endl;
}

void testParser(bool equal, Node* expr, const Node* expected, Settings* settings) {
    std::cerr << "Test if parsing\n";
    expr->displayNexts(std::cerr);
    if (equal) std::cerr << "equals to\n";
    else std::cerr << "differs from\n";
    expr->display(std::cerr);
    std::cerr << ": ";
    try {
        Node* result = Parser(expr, settings).getFinalTree();
        if (areSameNodes(result, expected) == equal) std::cerr << "OK";
        else std::cerr << "KO";
        delete result;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed with error : " << e.what();
    }
    std::cerr << std::endl;
}

void testTokenizerAndParser(bool equal, const std::string &expr, const Node* expected, Settings* settings) {
    std::cerr << "Test if tokenizing and parsing\n'\n" << expr << "\n'\n";
    if (equal) std::cerr << "equals to\n";
    else std::cerr << "differs from\n";
    expected->display(std::cerr);
    std::cerr << ": ";
    try {
        Node* tokens = Tokenizer(expr, settings).getResult();
        Node* result = Parser(tokens, settings).getFinalTree();
        if (areSameNodes(result, expected) == equal) std::cerr << "OK";
        else {
            if (settings->debug) {
                std::cerr << "Final tree is:\n";
                result->display(std::cerr);
                std::cerr << "\n";
            }
            std::cerr << "KO";
        }
        delete result;
        delete tokens;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed with error : " << e.what();
    }
    std::cerr << std::endl;
}

std::string testEqualBools(bool equal, bool a, bool b) {
    if ((a == b) == equal) return "OK";
    return "Error, " + std::to_string(a) + " instead of " + std::to_string(b);
}

std::string testEqualStrings(bool equal, const std::string &a, const std::string &b) {
    if ((a == b) == equal) return "OK";
    return "Error, " + a + " instead of " + b;
}

void testInvalidExpression(std::string expression, Settings* settings) {
    std::cerr << "Test if tokenizing and parsing\n'\n" << expression << "\n'\n raises a MalformedExpression exception : ";
    Node* tokens = nullptr;
    Node* result = nullptr;
    try {
        tokens = Tokenizer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        std::cerr << "KO";
    }
    catch (const MalformedExpression &e) {
        std::cerr << "OK";
    }
    catch (const std::exception &e) {
        std::cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    std::cerr << std::endl;
}

void testUnknownToken(std::string expression, Settings* settings) {
    std::cerr << "Test if tokenizing and parsing\n'\n" << expression << "\n'\n raises a UnknownToken exception : ";
    Node* tokens = nullptr;
    Node* result = nullptr;
    try {
        tokens = Tokenizer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        std::cerr << "KO";
    }
    catch (const UnknownToken &e) {
        std::cerr << "OK";
    }
    catch (const std::exception &e) {
        std::cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    std::cerr << std::endl;
}

void testMissingToken(std::string expression, Settings* settings) {
    std::cerr << "Test if tokenizing and parsing\n'\n" << expression << "\n'\n raises a MissingToken exception : ";
    Node* tokens = nullptr;
    Node* result = nullptr;
    try {
        tokens = Tokenizer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        std::cerr << "KO";
    }
    catch (const MissingToken &e) {
        std::cerr << "OK";
    }
    catch (const std::exception &e) {
        std::cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    std::cerr << std::endl;
}

void testUnknownValue(std::string expression, Settings* settings) {
    std::cerr << "Test if tokenizing and parsing\n'\n" << expression << "\n'\n raises a UnknownValue exception : ";
    Node* tokens = nullptr;
    Node* result = nullptr;
    try {
        tokens = Tokenizer(expression, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        std::cerr << "KO";
    }
    catch (const UnknownValue &e) {
        std::cerr << "OK";
    }
    catch (const std::exception &e) {
        std::cerr << "Error : " << e.what();
    }
    delete tokens;
    delete result;
    std::cerr << std::endl;
}

std::string getFileContent(std::string fileName) {
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    Settings* settings = new Settings;

    std::string fileContent;

    Node* rootExpected;
    Node* expected;

    settings->debug = false;

    fileContent = getFileContent("src/style/tests/test-1.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockPrototype});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "background-color"});
    expected->appendChild(new Node{Token::String, "#ff0000"});

    testTokenizerAndParser(true, fileContent, rootExpected, settings);

    delete rootExpected;
    expected = nullptr;

    settings->debug = false;

    fileContent = getFileContent("src/style/tests/test-2.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockPrototype});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::String, "#0000ff"});

    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockPrototype});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected->appendChild(new Node{Token::Modifier, "hovered"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    testTokenizerAndParser(true, fileContent, rootExpected, settings);

    delete rootExpected;
    expected = nullptr;

    settings->debug = false;

    fileContent = getFileContent("src/style/tests/test-3.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockPrototype});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::String, "#0000ff"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockPrototype});
    expected->appendChild(new Node{Token::Modifier, "hovered"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    testTokenizerAndParser(true, fileContent, rootExpected, settings);

    delete rootExpected;
    expected = nullptr;

    settings->debug = false;

    fileContent = getFileContent("src/style/tests/test-4.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockPrototype});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::String, "#0000ff"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockPrototype});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    testTokenizerAndParser(true, fileContent, rootExpected, settings);

    delete rootExpected;
    expected = nullptr;


    delete settings;
    return 0;
}
