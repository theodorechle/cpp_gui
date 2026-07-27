- add tab control (next, previous) + allow specifying an order
- check if SDL3 supports multiple render depths (forward, backward, ...) without needing a redraw of the elements behind an other
- allow re-computing the layout only of modified elements and their parents

- Recompute on size changing (modifiers, ...)
- add width and height auto (100% parent)
- force style flag file reloading when receiving it twice

- Simplify the use of the text engine

- fix resize

- update elements on events (hovered, clicked, ...) only if they do something (have a modifier, maybe other things)
- don't re-render elements who don't change (only the ones who changes and their parents)

- don't redraw parents if size don't change (draw directly, bypass parent)

- Prevent having multiple times submodule cpp_tests -> dynamic library loading

- Shift + scroll -> horizontal scroll

- Fix inconsistencies on namespaces (for example, some classes are in ui and others not)

- Search elements using CSS like selectors/path/XPath (name, id, class, ...)

- rename childs -> children

- asynchronous:
  - [x] event thread safe queue for each element
  - [ ] UiManager should tell each element if it is currently being redrawed, in which case it cannot process events (/!\ ensure all elements to redraw are known before starting to redraw them, for example parents who could need to be redrawed if a child changed size) -> needed for parallel computing, for the beginning they could juste be forbidden to change anything while re-render is occuring
  - [ ] every alteration of an element must occur through the event handler so it is centralized and the lock discussed earlier can be set (find a way to do it without using the gui becoming less intuitive (I think of adding a child for example, which is cleaner with `element->addChild(child)` than `uiManager->addChild(element, child)` even tho it's still usable)) -> no problem, element->addChild adds an event to the element queue, the manager don't need to know it
  - [x] thread safe set in abstract manager for elements to re-render
  - [ ] ensure elements aren't accessed when being deleted (child, next, parent)
  - [ ] ensure no event is added to an element queue when this element is being deleted
  - [ ] add threads to run elements event handlers (and manager update?):
    - thread pool?
    - a thread for multiple elements?
    - a thread per element?
