// =======================================================================
// Utilities.h
// -----------------------------------------------------------------------
// Declares small helper functions used throughout the project:
//   - removing spaces from the raw input
//   - checking if a character is an operand (A-Z) or an operator
//   - getting operator precedence (for infix -> postfix conversion)
//   - validating the whole expression before we process it
// These functions are defined in Utilities.cpp
// =======================================================================

#ifndef UTILITIES_H   // header guard: prevents this file being included twice
#define UTILITIES_H

#include <string>
using namespace std;

// Removes all spaces/tabs from a string and returns the cleaned string.
// Example: "A = B + C" -> "A=B+C"
string removeSpaces(const string &expr);

// Returns true if 'ch' is a single uppercase letter A-Z (a valid operand).
bool isOperand(char ch);

// Returns true if 'ch' is one of the four supported arithmetic operators.
bool isOperatorChar(char ch);

// Returns the precedence rank of an operator.
// '*' and '/' have higher precedence (2) than '+' and '-' (1).
// Returns 0 for anything that is not +,-,*,/  (used for '(' as a sentinel).
int getPrecedence(char op);

// Validates the FULL expression (e.g. "A=(B+C)*D-E/F").
// Returns true if the expression is valid.
// If it is NOT valid, returns false and fills 'errorMessage' with the
// exact reason, so the user gets a meaningful error on screen.
bool validateExpression(const string &expr, string &errorMessage);

#endif // UTILITIES_H
