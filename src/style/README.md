# style
A program who pick files in a css like format and convert them to understandable style for the cpp_gui.

## FEATURES:
### Style types (expressed in pseudo regex)
- int: [0-9]+
- float: [0-9]+\.[0-9]\*
- string: [^,;\s{}()[]]+
- tuple[type,...]: \\( *\<style-type\>( \*, \*\<style-type\>)\* \*\\)
- bool

### Operators
- \+ addition
- \- substraction
- \* multiplication
- \/ division

### Functions
regex: [\w\d][\w\d\\-]+[\w\d]
- round(int|float)

#### Units
An int can be followed by:
 - % to apply a percentage of the parent size (automatically choose width or height)
 - pw to apply a percentage of the parent width
 - ph to apply a percentage of the parent height
 - ww to apply a percentage of the window width
 - wh to apply a percentage of the window height
 - px to be in pixels (default)

#### ! support custom styles

### Modifiers
Shown in precedence priority order:

focused
</br>
clicked
</br>
hovered

Custom modifiers can be written

### Identifiers (single element)
An identifier (id) can be used to apply to a specific element

### Classes (groups of elements)
A class can be used to apply style to multiple elements at once

### Relation between elements
- direct parent
- any parent
- same element

## SYNTAX:

### Syntax

```
<element-name|.class|#id><['>'|' ']<.class|#id|:modifier>| element-name>[...][, ...] { 
    <style-name>: <string|int|float|bool|tuple[string|int|float|bool,...]>[,...];
    <style-name>: <int|float> +|-|/|*|% <int|float>;
    <style-name>: function(<string|int|float|bool|tuple[string|int|float|bool,...]>, ...);

    <element-name|.class|#id|:modifier><<.class|#id|:modifier>| element-name>[...] {
        // recursive (styles, inner elements|classes|ids|modifiers)
        [...]
    }
    // comment
    /*
    multiline comment
    */
}
```

### Examples
<a href="../../tests/style_tests_lexer_and_parser/tests-files">See example files</a>

## Others:
Note that to many recursive style blocks can lead to a stack overflow error.