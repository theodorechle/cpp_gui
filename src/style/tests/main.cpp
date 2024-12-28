#include "style_tests.hpp"

void StyleTests::tests() {
    std::string fileContent;

    Node* rootExpected;
    Node* expected;

    settings->debug = false;
    fileContent = getFileContent("src/style/tests/tests/test-1.txt");
    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "background-color"});
    expected->appendChild(new Node{Token::String, "#ff0000"});

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;


    settings->debug = false;
    fileContent = getFileContent("src/style/tests/tests/test-2.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::String, "#0000ff"});

    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected->appendChild(new Node{Token::Modifier, "hovered"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;


    settings->debug = false;
    fileContent = getFileContent("src/style/tests/tests/test-3.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::String, "#0000ff"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::Modifier, "hovered"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;


    settings->debug = false;
    fileContent = getFileContent("src/style/tests/tests/test-4.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::String, "#0000ff"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;


    settings->debug = false;
    fileContent = getFileContent("src/style/tests/tests/test-5.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected->appendChild(new Node{Token::Modifier, "hovered"});
    expected->appendChild(new Node{Token::DirectParent});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected->appendChild(new Node{Token::Class, "red"});
    expected->appendChild(new Node{Token::Identifier, "root"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::String, "#0000ff"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;


    settings->debug = false;
    fileContent = getFileContent("src/style/tests/tests/test-6.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "label"});
    expected->appendChild(new Node{Token::Class, "blue"});
    expected->appendChild(new Node{Token::Modifier, "hovered"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected->appendChild(new Node{Token::Class, "red"});
    expected->appendChild(new Node{Token::Identifier, "root"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected->appendChild(new Node{Token::String, "#0000ff"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "text-color"});
    expected = expected->appendChild(new Node{Token::Tuple});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});
    expected->appendChild(new Node{Token::Int, "150"});

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;


    settings->debug = false;
    fileContent = getFileContent("src/style/tests/tests/test-7.txt");
    invalidExpression(fileContent);


    settings->debug = true;
    fileContent = getFileContent("src/style/tests/tests/test-8.txt");

    rootExpected = new Node{Token::NullRoot};
    expected = rootExpected->appendChild(new Node{Token::StyleBlock});
    expected = expected->appendChild(new Node{Token::BlockDeclaration});
    expected = expected->appendChild(new Node{Token::Declaration});
    expected->appendChild(new Node{Token::ElementName, "element"});
    expected = expected->getParent();
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::BlockDefinition});
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "width"});
    expected->appendChild(new Node{Token::PixelUnit})->appendChild(new Node{Token::Int, "150"});
    expected = expected->getParent();
    expected = expected->appendChild(new Node{Token::Assignment});
    expected->appendChild(new Node{Token::StyleName, "height"});
    expected->appendChild(new Node{Token::PercentageUnit})->appendChild(new Node{Token::Int, "40"});

    lexerAndParser(true, fileContent, rootExpected);

    delete rootExpected;
    expected = nullptr;

    settings->debug = false;
    fileContent = getFileContent("src/style/tests/tests/test-9.txt");
    invalidExpression(fileContent);
}

int main() {
    Settings *settings = new Settings();
    settings->debug = true;
    StyleTests test = StyleTests(settings);
    test.runTests();
    delete settings;
    return test.getNbErrors();
}
