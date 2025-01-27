### layout computing:
- compute wanted size
    - get wanted childs size
    - add padding
    - add self size
    - adjust with min and max sizes
- compute parent layout
- adjust with parent size




-> element ask child his desired size
    -> ...
-> add it's own size to his child size
-> adapt size to constraints
-> return to the parent

-> element adjust is own size with parent size
    -> ...

### place elements:
X don't mess with margins for now - compute position with margins
- get window size
- clip element (not sure it's needed)
- ask element to draw itself given computed position
