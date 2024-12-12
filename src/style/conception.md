An UIManager who manage all elements.
Elements in tree (DOM like).

Applicating style to elements:
Identifiers:
-map string, UIElement
Classes:
-map string, list\<UIElement\>
Modifiers:
-map string, list\<UIElement\>
Style inheritance:
-save parent in child

in element:
-map string, StyleObject

namespace style
Object:
Int
Float
String
Bool
Tuple

StyleMap

lot of maps:
 + memory

StyleComponent
    name: "element"
    type: type
    styles: [
        ((("blue", type, relation), ...), {"key": ("value", type), ...}),
        ...
    ]

relation -> direct parent, any parent, ...