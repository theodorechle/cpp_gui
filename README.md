![build status](https://github.com/theodorechle/cpp_gui/actions/workflows/c-cpp.yml/badge.svg)

# cpp_gui
A cpp replacement and improvement of my ![python_gui](https://github.com/theodorechle/python_gui)

This GUI is made on top of the ![SDL3 library](https://wiki.libsdl.org/SDL3/FrontPage)

![Readme for the gui's style](src/style/README.md)

# Installation

To use this program, you need g++ and make.

![Install SDL](https://github.com/libsdl-org/SDL/blob/main/INSTALL.md)

![Install SDL_ttf](https://github.com/libsdl-org/SDL_ttf/blob/main/INSTALL.md)

## Elements
The gui is designed so you can implements easily your own elements

### Abstracts
- <i>AbstractElement</i>
- <i>AbstractContainer</i>
- <i>UIElement</i>

### Concretes (they all inherits the AbstractElement class)
- Label
- Input
- Container
- Button
- TextButton
- ImageButton
- List
- Table
- Slider


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


## Manager
The Manager is a class who manages all the given elements.
It chooses when their rendering are computed and when they are rendered.
It knows the window size and state (focused, ...).
It activates and deactivates the style modifiers.

### Abstract
- <i>AbstractManager</i>

### Concrete
- UIManager
