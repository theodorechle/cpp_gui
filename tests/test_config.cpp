#include "test_config.hpp"

style::config::Config *testConfig() {
    return new style::config::Config{
        {{"padding", {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"padding-left",
          {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"padding-right",
          {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"padding-top",
          {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"padding-bottom",
          {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},

         {"border", {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"border-left",
          {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"border-right",
          {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"border-top",
          {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"border-bottom",
          {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},

         {"border-color", {new style::config::ConfigRuleNode(style::parser::Token::Hex)}},
         {"border-left-color", {new style::config::ConfigRuleNode(style::parser::Token::Hex)}},
         {"border-right-color", {new style::config::ConfigRuleNode(style::parser::Token::Hex)}},
         {"border-top-color", {new style::config::ConfigRuleNode(style::parser::Token::Hex)}},
         {"border-bottom-color", {new style::config::ConfigRuleNode(style::parser::Token::Hex)}},

         {"width", {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"height", {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"max-height",
          {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"min-height",
          {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},

         {"max-width", {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"min-width", {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},

         {"horizontal-alignment", {new style::config::ConfigRuleNodeEnum({"start", "centered", "end"})}},
         {"vertical-alignment", {new style::config::ConfigRuleNodeEnum({"start", "centered", "end"})}},
         {"vertical", {new style::config::ConfigRuleNode(style::parser::Token::Bool)}},

         {"background-color",
          {new style::config::ConfigRuleNode(style::parser::Token::Hex),
           new style::config::ConfigRuleNode(
               style::parser::Token::Tuple,
               new style::config::ConfigRuleNode(style::parser::Token::Int, nullptr,
                                                 new style::config::ConfigRuleNode(style::parser::Token::Int, nullptr,
                                                                                   new style::config::ConfigRuleNode(style::parser::Token::Int))))}},

         {"text-color",
          {new style::config::ConfigRuleNode(style::parser::Token::Hex),
           new style::config::ConfigRuleNode(
               style::parser::Token::Tuple,
               new style::config::ConfigRuleNode(style::parser::Token::Int, nullptr,
                                                 new style::config::ConfigRuleNode(style::parser::Token::Int, nullptr,
                                                                                   new style::config::ConfigRuleNode(style::parser::Token::Int))))}},

         {"scroll-bar-color",
          {new style::config::ConfigRuleNode(style::parser::Token::Hex),
           new style::config::ConfigRuleNode(
               style::parser::Token::Tuple,
               new style::config::ConfigRuleNode(style::parser::Token::Int, nullptr,
                                                 new style::config::ConfigRuleNode(style::parser::Token::Int, nullptr,
                                                                                   new style::config::ConfigRuleNode(style::parser::Token::Int))))}},
         {"scroll-bar-size",
          {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"overflow-x", {new style::config::ConfigRuleNodeEnum({"hidden", "scroll", "auto"})}},
         {"overflow-y", {new style::config::ConfigRuleNodeEnum({"hidden", "scroll", "auto"})}},

         {"text-wrap", {new style::config::ConfigRuleNodeEnum({"wrapped", "line-break"})}},
         {"font-size", {new style::config::ConfigRuleNode(style::parser::Token::Unit, new style::config::ConfigRuleNode(style::parser::Token::Int))}},
         {"font-name", {new style::config::ConfigRuleNode(style::parser::Token::String)}},
         {"font-weight", {new style::config::ConfigRuleNodeEnum({"normal", "bold"})}},
         {"font-italic", {new style::config::ConfigRuleNode(style::parser::Token::Bool)}},
         {"font-underline", {new style::config::ConfigRuleNode(style::parser::Token::Bool)}},
         {"font-strike-through", {new style::config::ConfigRuleNode(style::parser::Token::Bool)}}},
        {"px", "%"},
        {"font-size", "font-name", "font-weight", "font-italic", "font-underline", "font-strike-through", "text-color"}};
}
