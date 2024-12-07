# cpp_gui
A cpp replacement for the python_gui

# Installation

Install SDL
```sh
git clone https://github.com/libsdl-org/SDL
cd SDL
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release --parallel
sudo cmake --install . --config Release
```

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
