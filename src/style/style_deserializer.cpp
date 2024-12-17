#include "style_deserializer.hpp"

std::list<StyleComponent *> * StyleDeserializer::deserialize(const std::string & fileName) {
    Settings *settings = new Settings();
    std::ifstream file(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();

    Node* tokens = Lexer(buffer.str(), settings).getResult();
    Node* result = Parser(tokens, settings).getFinalTree();
    return NodeToStyleComponentList().convert(result);
}