#include "tokenizer.hpp"
#include "parser.hpp"
#include "solver.hpp"

using namespace std;

int main() {
    Settings *settings = new Settings;
    string *expr = new string;
    Node *tokens;
    cout << "Expression : ";
    getline(cin, *expr);
    cout << "1 for debug, 0 else : ";
    cin >> settings->debug;
    cout << "1 for implicit multiplication priority, 0 else : ";
    cin >> settings->implicitMultiplicationPriority;
    Tokenizer *tokenizer = new Tokenizer{*expr, settings};
    tokens = tokenizer->getResult();
    delete tokenizer;
    if (settings->debug) {
        cerr << "Tokens" << endl;
        tokens->displayNexts(cerr);
    }
    Node *tree;
    Parser *parser = new Parser{tokens, settings};
    tree = parser->getFinalTree();
    delete parser;
    delete tokens;
    if (tree == nullptr) { // something is not freed, need to find it
        cout << "Empty result" << endl;
        delete expr;
        delete tree;
        delete settings;
        return 0;
    }
    if (settings->debug) {
        cerr << "Parsed tree" << endl;
        tree->display(cerr);
        cerr << tree->str() << endl;
    }
    tree = solve(tree, settings->debug);
    if (settings->debug) {
        cerr << "Solved tree" << endl;
        tree->display(cerr);
    }
    cout << "Start expression : " << endl << *expr << endl;
    cout << "Final expression : " << endl << *tree << endl;
    delete expr;
    delete tree;
    delete settings;
    return 0;
}
