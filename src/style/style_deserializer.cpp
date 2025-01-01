#include "style_deserializer.hpp"

std::list<StyleComponent *> *StyleDeserializer::deserializeFromFile(const std::string &fileName, int fileNumber, int *ruleNumber) {
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();

    return deserialize(buffer.str(), fileNumber, ruleNumber);
}

std::list<StyleComponent *> *StyleDeserializer::deserialize(const std::string &style, int fileNumber, int *ruleNumber) {
    std::list<StyleComponent *> *deserializedStyle;
    Settings *settings = new Settings();
    settings->debug = false;
    Node *tokens = Lexer(style, settings).getResult();
    Node *result = Parser(tokens, settings).getFinalTree();
    delete tokens;
    delete settings;
    deserializedStyle = NodeToStyleComponentList().convert(result, fileNumber, ruleNumber);
    delete result;
    return deserializedStyle;
}