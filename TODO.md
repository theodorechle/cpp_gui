- add tab control (next, previous) + allow specifying an order
- check if SDL3 supports multiple render depths (forward, backward, ...) without needing a redraw of the elements behind an other
- allow re-computing the layout only of modified elements and their parents

Recompute on size changing (modifiers, ...)
add overflow (scroll)
add width and height auto (100% parent)
force style flag file reloading when receiving it twice


Rendering:
clipping

- border OVERRIDABLE
- background OVERRIDABLE
- rendering before child OVERRIDABLE
- render childs OVERRIDABLE
- rendering after child OVERRIDABLE

Simplify the use of the text engine
Update namespaces

merge all trees into one class to reduce bugs and simplify tests

fix resize

update elements on events (hovered, clicked, ...) only if they do something (have a modifier, maybe other things)
don't re-render elements who don't change (only the ones who changes and their parents)

don't redraw parents if size don't change (draw directly, bypass parent)

Prevent having multiple times submodule cpp_tests

redraw doesn't works well (see input redraw)