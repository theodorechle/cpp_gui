- add tab control (next, previous) + allow specifying an order
- check if SDL3 supports multiple render depths (forward, backward, ...) without needing a redraw of the elements behind an other
- allow re-computing the layout only of modified elements and their parents

Recompute on size changing (modifiers, ...)
add overflow (scroll)
add width and height auto (100% parent)
force style file reloading when receiving it twice


Rendering:
clipping

- border
- background
- rendering before child OVERRIDABLE
- render childs
- rendering after child OVERRIDABLE

Simplify the use of the text engine
Update namespaces