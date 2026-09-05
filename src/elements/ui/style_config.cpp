#include "style_config.hpp"

namespace style::config {
    Config *config() {
        return new Config{{{"padding", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"padding-left", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"padding-right", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"padding-top", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"padding-bottom", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},

                           {"border", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"border-left", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"border-right", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"border-top", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"border-bottom", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},

                           {"border-color", {new ConfigRuleNode(style::parser::Token::Hex)}},
                           {"border-left-color", {new ConfigRuleNode(style::parser::Token::Hex)}},
                           {"border-right-color", {new ConfigRuleNode(style::parser::Token::Hex)}},
                           {"border-top-color", {new ConfigRuleNode(style::parser::Token::Hex)}},
                           {"border-bottom-color", {new ConfigRuleNode(style::parser::Token::Hex)}},

                           {"width", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"height", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"max-height", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"min-height", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},

                           {"max-width", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"min-width", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},

                           {"horizontal-alignment", {new ConfigRuleNodeEnum({"start", "centered", "end"})}},
                           {"vertical-alignment", {new ConfigRuleNodeEnum({"start", "centered", "end"})}},
                           {"vertical", {new ConfigRuleNode(style::parser::Token::Bool)}},

                           {"background-color",
                            {new ConfigRuleNode(style::parser::Token::Hex),
                             new ConfigRuleNode(style::parser::Token::Tuple,
                                                new ConfigRuleNode(style::parser::Token::Int, nullptr,
                                                                   new ConfigRuleNode(style::parser::Token::Int, nullptr,
                                                                                      new ConfigRuleNode(style::parser::Token::Int))))}},

                           {"text-color",
                            {new ConfigRuleNode(style::parser::Token::Hex),
                             new ConfigRuleNode(style::parser::Token::Tuple,
                                                new ConfigRuleNode(style::parser::Token::Int, nullptr,
                                                                   new ConfigRuleNode(style::parser::Token::Int, nullptr,
                                                                                      new ConfigRuleNode(style::parser::Token::Int))))}},

                           {"scroll-bar-color",
                            {new ConfigRuleNode(style::parser::Token::Hex),
                             new ConfigRuleNode(style::parser::Token::Tuple,
                                                new ConfigRuleNode(style::parser::Token::Int, nullptr,
                                                                   new ConfigRuleNode(style::parser::Token::Int, nullptr,
                                                                                      new ConfigRuleNode(style::parser::Token::Int))))}},
                           {"scroll-bar-size", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"overflow-x", {new ConfigRuleNodeEnum({"hidden", "scroll", "auto"})}},
                           {"overflow-y", {new ConfigRuleNodeEnum({"hidden", "scroll", "auto"})}},

                           {"text-wrap", {new ConfigRuleNodeEnum({"wrapped", "line-break"})}},
                           {"font-size", {new ConfigRuleNode(style::parser::Token::Unit, new ConfigRuleNode(style::parser::Token::Int))}},
                           {"font-name", {new ConfigRuleNode(style::parser::Token::String)}},
                           {"font-weight", {new ConfigRuleNodeEnum({"normal", "bold"})}},
                           {"font-italic", {new ConfigRuleNode(style::parser::Token::Bool)}},
                           {"font-underline", {new ConfigRuleNode(style::parser::Token::Bool)}},
                           {"font-strike-through", {new ConfigRuleNode(style::parser::Token::Bool)}}},
                          {"px", "%"},
                          {"font-size", "font-name", "font-weight", "font-italic", "font-underline", "font-strike-through", "text-color"}};
    }
} // namespace style::config
