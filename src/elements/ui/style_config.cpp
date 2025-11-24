#include "style_config.hpp"

namespace style::config {
    Config *config() {
        return new Config{
            {{"padding", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},
             {"padding-left", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},
             {"padding-right", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},
             {"padding-top", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},
             {"padding-bottom", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},

             {"border", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},
             {"border-left", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},
             {"border-right", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},
             {"border-top", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},
             {"border-bottom", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},

             {"border-color", {new ConfigRuleNode(style::Token::Hex)}},
             {"border-left-color", {new ConfigRuleNode(style::Token::Hex)}},
             {"border-right-color", {new ConfigRuleNode(style::Token::Hex)}},
             {"border-top-color", {new ConfigRuleNode(style::Token::Hex)}},
             {"border-bottom-color", {new ConfigRuleNode(style::Token::Hex)}},

             {"width", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},
             {"height", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},
             {"max-height", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},
             {"min-height", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},

             {"max-width", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},
             {"min-width", {new ConfigRuleNode(style::Token::Unit, new ConfigRuleNode(style::Token::Int))}},

             {"horizontal-alignment", {new ConfigRuleNodeEnum({"start", "centered", "end"})}},
             {"vertical-alignment", {new ConfigRuleNodeEnum({"start", "centered", "end"})}},
             {"vertical", {new ConfigRuleNode(style::Token::Bool)}},

             {"background-color",
              {new ConfigRuleNode(style::Token::Hex),
               new ConfigRuleNode(style::Token::Tuple,
                                  new ConfigRuleNode(style::Token::Int, nullptr,
                                                     new ConfigRuleNode(style::Token::Int, nullptr, new ConfigRuleNode(style::Token::Int))))}},

             {"text-color",
              {new ConfigRuleNode(style::Token::Hex),
               new ConfigRuleNode(style::Token::Tuple,
                                  new ConfigRuleNode(style::Token::Int, nullptr,
                                                     new ConfigRuleNode(style::Token::Int, nullptr, new ConfigRuleNode(style::Token::Int))))}},

             {"text-wrap", {new ConfigRuleNodeEnum({"wrapped", "line-break"})}},
             {"font-size", {new ConfigRuleNode(style::Token::Int)}},
             {"font-name", {new ConfigRuleNode(style::Token::String)}},
             {"font-weight", {new ConfigRuleNodeEnum({"normal", "bold"})}},
             {"font-italic", {new ConfigRuleNode(style::Token::Bool)}},
             {"font-underline", {new ConfigRuleNode(style::Token::Bool)}},
             {"font-strike-through", {new ConfigRuleNode(style::Token::Bool)}}},
            {"px", "%"}};
    }

} // namespace style::config
