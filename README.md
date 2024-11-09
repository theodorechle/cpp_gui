# cpp_gui
A cpp replacement for the python_gui


# STYLE FILES:

## FEATURES:
### Style types
int
float
string
tuple[type,...]
bool

#### Special
An int can be followed by a % to apply a percentage of the parent size.

### Default styles:
- UIElement:
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
- Label:
    - text-color: string|tuple[int,int,int]
    - font: string
    - font-size: int
    - antialias: bool
- InputTextBox:
    - caret-color: string|tuple[int,int,int]
    - placeholder-color: string|tuple[int,int,int]
- Slider:
    - bar-color: string|tuple[int,int,int]
    - cursor-color: string|tuple[int,int,int]
- Container:
- ItemList:
- Button:
- Table:

#### ! support custom styles

### Modifiers
Shown in the order style would apply

focused
clicked
hovered

### Ids (single element)
An id can be used to apply to a specific element

### Classes (groups of elements)
A class can be used to apply style to multiple elements at once

### Relation between elements
- direct child
- distant child
- direct parent
- distant parent

## SYNTAX:
```
"<element-name|.class|#id><<.class|#id>| element-name>[...][, ...]" { 
    <style-name>: <styleType>[,...];
}
```

### Examples