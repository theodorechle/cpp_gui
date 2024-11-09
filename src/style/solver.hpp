#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "node.hpp"

void findSuperiorGroup(Node *node, int priority, Node *nodes);

Node *goToFirstLeaf(Node *node);

/**
  *Go up in the tree and returns the next child to solve (use the childIndex)
*/
Node *goToNextExpr(Node *node);

Node *solve(Node *expr, bool debug);

#endif // SOLVER_HPP