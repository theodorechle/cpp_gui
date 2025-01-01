#include "style_deserializer.hpp"

std::list<StyleBlock *> *StyleDeserializer::deserializeFromFile(const std::string &fileName, int fileNumber, int *ruleNumber, bool debug) {
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();

    return deserialize(buffer.str(), fileNumber, ruleNumber, debug);
}

std::list<StyleBlock *> *StyleDeserializer::deserialize(const std::string &style, int fileNumber, int *ruleNumber, bool debug) {
    std::list<StyleBlock *> *deserializedStyle = nullptr;
    Node *tokens = nullptr;
    Node *result = nullptr;
    Settings *settings = new Settings();
    settings->debug = debug;
    try {
        tokens = Lexer(style, settings).getResult();
        result = Parser(tokens, settings).getFinalTree();
        deserializedStyle = NodeToStyleComponentList().convert(result, fileNumber, ruleNumber);
    }
    catch (...) {
        delete tokens;
        delete result;
        delete settings;
        throw;
    }
    delete tokens;
    delete result;
    delete settings;
    return deserializedStyle;
}