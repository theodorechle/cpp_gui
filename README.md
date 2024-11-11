# cpp_gui
A cpp replacement for the python_gui


# STYLE FILES:

## FEATURES:
### Style types (exprimed in pseudo regex)
- int: [0-9]+
- float: [0-9]+\.[0-9]\*
- string: [^,;\s{}()[]]+
- tuple[type,...]: \\( *\<style-type\>(, \*\<style-type\>)\* \*\\)
- bool

### Operators
- \+ addition
- \- substraction
- \* multiplication
- \/ division

### Functions
regex: [\w\d][\w\d\\-]+[\w\d]
- round(int|float)

#### Special
An int can be followed by:
 - % to apply a percentage of the parent size (automatically choose width or height)
 - pw to apply a percentage of the parent width
 - ph to apply a percentage of the parent height
 - ww to apply a percentage of the window width
 - wh to apply a percentage of the window height
 - px to be in pixels (default)

### Default styles:
- element: // UIElement
    - background-color: string|tuple[int,int,int]
    - border-color: string|tuple[int,int,int]
    - border-width: int
    - border-radius: int
    - border-top-left-radius: int
    - border-top-right-radius: int
    - border-bottom-left-radius: int
    - border-bottom-right-radius: int
    - height: int
    - min-height: int
    - max-height: int
    - width: int
    - min-width: int
    - max-width: int
    - padding: int
- label: // Label
    - text-color: string|tuple[int,int,int]
    - font: string
    - font-size: int
    - antialias: bool
- input: // InputTextBox
    - caret-color: string|tuple[int,int,int]
    - placeholder-color: string|tuple[int,int,int]
- slider: // Slider
    - bar-color: string|tuple[int,int,int]
    - cursor-color: string|tuple[int,int,int]
- container: // Container
- list: // ItemList
- button: // Button
- table: // Table

#### ! support custom styles

### Modifiers
Shown in the order style would apply

focused
clicked
hovered

### Identifiers (single element)
An ididentifier (id) can be used to apply to a specific element

### Classes (groups of elements)
A class can be used to apply style to multiple elements at once

### Relation between elements
- direct child
- distant child
- direct parent
- distant parent

## SYNTAX:

### Syntax

```
<element-name|.class|#id><<.class|#id|:modifier>| element-name>[...][, ...] { 
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
```
element { // ui elements will have a red background
    background-color: #ff0000;
}

label.blue { // labels with the class 'blue' will have a text in blue
    text-color: #0000ff;
}

label.blue:hovered {
    /*
    labels with the class 'blue' will have a text in gray
    this override the previous declaration block of 'label.blue' when it's hovered
    */
    text-color: (150, 150, 150);
}

// equivalent at previous two blocks
label.blue {
    text-color: #0000ff;
    :hovered {
        text-color: #888888;
    }
}
```