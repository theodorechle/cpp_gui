#include "tokenizer.hpp"
#include "parser.hpp"
#include "solver.hpp"

#include <sstream>
#include <fstream>

using namespace std;

int main() {
    Settings* settings = new Settings;
    string* fileName = new string;
    string* fileContent = new string;
    Tokenizer* tokenizer = nullptr;
    Parser* parser = nullptr;
    Node* tokens;
    Node* tree;

    while (true) {
        cout << "Style file : ";
        getline(cin,* fileName);
        ifstream file(fileName->c_str());
        stringstream buffer;
        buffer << file.rdbuf();
        *fileContent = buffer.str();
        if (fileContent->size()) break;
        cout << "Invalid file name" << endl;
    }
    cout << "File content:\n" <<* fileContent << endl;
    cout << "1 for debug, 0 else : ";
    cin >> settings->debug;
    try {
        tokenizer = new Tokenizer{*fileContent, settings};
    } catch (const TokenizerError &) {
        delete tokenizer;
        delete fileContent;
        delete fileName;
        delete settings;
        throw;
    }
    tokens = tokenizer->getResult();
    delete tokenizer;
    if (settings->debug) {
        cerr << "Tokens" << endl;
        tokens->displayNexts(cerr);
    }

    try {
        parser = new Parser{tokens, settings};
    } catch (const ParserError &e) {
        delete parser;
        delete fileContent;
        delete fileName;
        delete tokens;
        delete settings;
        throw;
    }
    tree = parser->getFinalTree();
    delete parser;
    delete tokens;
    if (tree == nullptr) { // something is not freed, need to find it
        cout << "Empty result" << endl;
        delete fileContent;
        delete fileName;
        delete tree;
        delete settings;
        throw;
    }
    if (settings->debug) {
        cerr << "Parsed tree" << endl;
        tree->display(cerr);
    }
    // tree = solve(tree, settings->debug);
    // if (settings->debug) {
    //     cerr << "Solved tree" << endl;
    //     tree->display(cerr);
    // }
    delete fileContent;
    delete fileName;
    delete tree;
    delete settings;
    return 0;
}
