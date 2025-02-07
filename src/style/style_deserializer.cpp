#include "style_deserializer.hpp"

namespace style {

    std::list<StyleBlock *> *StyleDeserializer::deserializeFromFile(const std::string &fileName, int fileNumber, int *ruleNumber, bool debug) {
        std::ifstream file(fileName);
        std::stringstream buffer;
        if (!file.is_open()) {
            std::cerr << "File '" << fileName << "' couldn't be opened\n";
            return nullptr;
        }
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
            deserializedStyle = NodeToStyleComponentList().convert(result, fileNumber, ruleNumber, debug);
        }
        catch (const ParserError &) {
            delete tokens;
            delete result;
            delete settings;
            throw;
        }
        catch (const LexerError &) {
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

} // namespace Style
