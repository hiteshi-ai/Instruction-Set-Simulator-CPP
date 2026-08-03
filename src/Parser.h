// =======================================================================
// Parser.h
// -----------------------------------------------------------------------
// Declares functions that:
//   1) split "A=B+C" into lhs="A" and rhs="B+C"
//   2) convert an infix expression (rhs) into postfix form using a stack
// Defined in Parser.cpp
// =======================================================================

#ifndef PARSER_H
#define PARSER_H

#include <string>
using namespace std;

// Splits the full expression at the '=' sign.
// 'lhs' receives the variable being assigned to (e.g. "A").
// 'rhs' receives the arithmetic expression (e.g. "B+C").
// Assumes 'expr' has already been validated (exactly one '=').
void splitExpression(const string &expr, string &lhs, string &rhs);

// Converts an infix expression (e.g. "(B+C)*D-E/F") into its
// postfix / Reverse Polish form (e.g. "BC+D*EF/-") using the
// classic stack-based Shunting-Yard style algorithm.
string infixToPostfix(const string &infix);

#endif // PARSER_H
