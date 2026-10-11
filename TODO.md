- add tab control (next, previous) + allow specifying an order => next and previous are done
- handlers (both in element and in manager):
  - add handler for when given key is pressed => replace tab current check with that
  - add handler for when given keys are pressed => useful for shift+tab for example
  Not urgent, since I've added pressedKeys set.
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

- Search elements using CSS like selectors/path/XPath (name, id, class, ...):
  - Same path syntax as CSS selectors

- Add an ID (more a typedef which can be a single value or a struct, so it can maybe evolve later) to elements to allow interacting with them

- rename children -> children
- previous draw not cleared properly when resizing an element on which size is set with percentages

- asynchronous:
  - [x] event thread safe queue for each element
  - [ ] UiManager should tell each element if it is currently being redrawed, in which case it cannot process events (/!\ ensure all elements to redraw are known before starting to redraw them, for example parents who could need to be redrawed if a child changed size) -> needed for parallel computing, for the beginning they could juste be forbidden to change anything while re-render is occuring
  - [ ] every alteration of an element must occur through the event handler so it is centralized and the lock discussed earlier can be set (find a way to do it without using the gui becoming less intuitive (I think of adding a child for example, which is cleaner with `element->addChild(child)` than `uiManager->addChild(element, child)` even tho it's still usable)) -> no problem, element->addChild adds an event to the element queue, the manager don't need to know it
  - [x] thread safe set in abstract manager for elements to re-render
  - [ ] ensure elements aren't accessed when being deleted (child, next, parent)
  - [ ] ensure no event is added to an element queue when this element is being deleted
  - [ ] more generally, ensure element is not accessed when being deleted (if a method is called while it's being deleted, it could be bad)
  - [ ] add threads to run elements event handlers (and manager update?):
    - thread pool?
    - a thread for multiple elements?
    - a thread per element?
  - requests to read, like events, can be synchronous using a future:
    ```cpp
    std::promise<Response> promise;
    std::future<Response> response = promise.get_future();

    // send request to element, which will then resolve the promise like
        promise->set_value(Response());
    // or if element is not found
        promise->set_exception(std::make_exception_ptr(std::runtime_error("element not found"))); // or custom error (better)

    // the manager waits using
    response.get();
    // and then return the result
    ```

With rvalues, I guess I could allow creating elements simply without giving user access to the element (since a rvalue means (to verify) it has been created in place, in the function call.
