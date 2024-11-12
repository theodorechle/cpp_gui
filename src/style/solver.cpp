#include "solver.hpp"
#include "parser.hpp"

void sortTree(Node *node) {
    
}

Node *goToFirstLeaf(Node *node) {
    if (node->getChild() != nullptr) return goToFirstLeaf(node->getChild());
    return node;
}

Node *goToNextExpr(Node *node) {
    if (node->getNext() != nullptr) return goToFirstLeaf(node->getNext());
    return node->getParent();
}

/* Node *solve(Node *expr, bool debug) {
    bool updated = true;
    bool delete_node = false;
    Operator *ope;
    while (updated || !Parser::isNodeNull(expr->getParent())) {
        if (updated) expr = goToFirstLeaf(expr);
        else expr = goToNextExpr(expr);
        delete_node = false;
        updated = false;
        ope = dynamic_cast<Operator*>(expr);
        if (ope != nullptr) updated = ope->solve(&delete_node);
        if (delete_node) {
            expr = ope->getChild();
            ope->setChild(nullptr);
            if (ope->getParent() != nullptr) {
                ope->getParent()->replaceChild(ope, expr);
            }
            else {
                expr->setParent(nullptr);
                delete ope;
            }
        }
    }
    return expr;
} */