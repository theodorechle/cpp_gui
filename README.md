![build status](https://github.com/theodorechle/cpp_gui/actions/workflows/c-cpp.yml/badge.svg)

# cpp_gui
A cpp improvement of the python_gui ![https://github.com/theodorechle/python_gui](https://github.com/theodorechle/python_gui)

![Readme for the gui's style](src/style/README.md)

# Installation

To use this program, you need g++ and make.

Install SDL

<a href="https://wiki.libsdl.org/SDL3/Installation">https://wiki.libsdl.org/SDL3/Installation</a>

## Elements
The gui is designed so you can implements easily your own elements

### Abstracts
- <i>AbstractElement</i>
- <i>AbstractContainer</i>

### Concretes (they all inherits the AbstractElement class)
- UIElement
- Label
- Input
- Container
- Button
- TextButton
- ImageButton
- List
- Table
- Slider

## Manager
The Manager is a class who manages all the given elements.
It chooses when their rendering are computed and when they are rendered.
It knows the window size and state (focused, ...).
It activates and deactivates the style modifiers.

### Abstract
- <i>AbstractManager</i>

### Concrete
- UIManager
