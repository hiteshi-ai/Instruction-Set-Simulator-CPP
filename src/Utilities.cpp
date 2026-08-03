// =======================================================================
// Utilities.cpp
// -----------------------------------------------------------------------
// Definitions for the helper functions declared in Utilities.h
// =======================================================================

#include "Utilities.h"
#include <cctype>   // for isspace(), isupper()

// -----------------------------------------------------------------------
// removeSpaces()
// Walks through every character of the input string. Any character that
// is NOT a space or tab is copied into the result string.
// -----------------------------------------------------------------------
string removeSpaces(const string &expr)
{
    string result = "";                       // will hold the cleaned string
    for (unsigned int i = 0; i < expr.size(); i++)
    {
        char ch = expr[i];                    // current character
        if (ch != ' ' && ch != '\t')          // keep everything except spaces/tabs
        {
            result += ch;
        }
    }
    return result;
}

// -----------------------------------------------------------------------
// isOperand()
// A valid operand in this project is a SINGLE uppercase letter A-Z.
// -----------------------------------------------------------------------
bool isOperand(char ch)
{
    return (ch >= 'A' && ch <= 'Z');
}

// -----------------------------------------------------------------------
// isOperatorChar()
// Checks whether the character is one of the four supported operators.
// -----------------------------------------------------------------------
bool isOperatorChar(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

// -----------------------------------------------------------------------
// getPrecedence()
// Higher number = higher precedence = evaluated first.
// * and / bind tighter than + and -
// -----------------------------------------------------------------------
int getPrecedence(char op)
{
    if (op == '+' || op == '-')
    {
        return 1;
    }
    if (op == '*' || op == '/')
    {
        return 2;
    }
    return 0;   // '(' or any other character -> lowest precedence
}

// -----------------------------------------------------------------------
// validateExpression()
// Performs a full set of checks on the cleaned expression and reports
// the FIRST problem found via 'errorMessage'. Returns true only if the
// expression passes every check.
// -----------------------------------------------------------------------
bool validateExpression(const string &expr, string &errorMessage)
{
    // ---- Check 1: expression must not be empty ----
    if (expr.empty())
    {
        errorMessage = "Expression is empty.";
        return false;
    }

    // ---- Check 2: exactly one '=' sign must be present ----
    int equalsCount = 0;
    int equalsPosition = -1;
    for (unsigned int i = 0; i < expr.size(); i++)
    {
        if (expr[i] == '=')
        {
            equalsCount++;
            equalsPosition = static_cast<int>(i);
        }
    }
    if (equalsCount == 0)
    {
        errorMessage = "Missing '=' sign. Expression must be of the form A=<expression>.";
        return false;
    }
    if (equalsCount > 1)
    {
        errorMessage = "More than one '=' sign found. Only one assignment is allowed.";
        return false;
    }

    // ---- Split into left-hand side (lhs) and right-hand side (rhs) ----
    string lhs = expr.substr(0, equalsPosition);
    string rhs = expr.substr(equalsPosition + 1);

    // ---- Check 3: left-hand side must be exactly ONE uppercase letter ----
    if (lhs.size() != 1 || !isOperand(lhs[0]))
    {
        errorMessage = "Left side of '=' must be a single uppercase letter (A-Z), e.g. A=...";
        return false;
    }

    // ---- Check 4: right-hand side must not be empty ----
    if (rhs.empty())
    {
        errorMessage = "Right side of '=' is empty. Nothing to compute.";
        return false;
    }

    // ---- Check 5: only allowed characters may appear on the right side ----
    for (unsigned int i = 0; i < rhs.size(); i++)
    {
        char ch = rhs[i];
        bool allowedChar = isOperand(ch) || isOperatorChar(ch) || ch == '(' || ch == ')';
        if (!allowedChar)
        {
            errorMessage = "Invalid character '" + string(1, ch) + "' found. Only A-Z, +, -, *, /, ( and ) are allowed.";
            return false;
        }
    }

    // ---- Check 6: parentheses must be balanced ----
    int parenBalance = 0;
    for (unsigned int i = 0; i < rhs.size(); i++)
    {
        if (rhs[i] == '(')
        {
            parenBalance++;
        }
        else if (rhs[i] == ')')
        {
            parenBalance--;
            // If balance ever goes negative, a ')' appeared before its matching '('
            if (parenBalance < 0)
            {
                errorMessage = "Unbalanced parentheses: a ')' appears without a matching '('.";
                return false;
            }
        }
    }
    if (parenBalance != 0)
    {
        errorMessage = "Unbalanced parentheses: missing closing ')'.";
        return false;
    }

    // ---- Check 7: token-by-token grammar check ----
    // We track the "category" of the previous character to catch things like:
    //   two operands in a row ("AB"), two operators in a row ("A++B"),
    //   an expression starting/ending with an operator, empty "()" pairs,
    //   an operator immediately after '(' or before ')', etc.
    // prevType values: 'S' = start, 'O' = operand, 'P' = operator, '(' , ')'
    char prevType = 'S';

    for (unsigned int i = 0; i < rhs.size(); i++)
    {
        char ch = rhs[i];

        if (isOperand(ch))
        {
            // An operand cannot immediately follow another operand or ')'
            if (prevType == 'O')
            {
                errorMessage = "Two operands appear together without an operator (e.g. \"AB\").";
                return false;
            }
            if (prevType == ')')
            {
                errorMessage = "An operand cannot immediately follow ')'. Insert an operator.";
                return false;
            }
            prevType = 'O';
        }
        else if (isOperatorChar(ch))
        {
            // An operator cannot be the first token, cannot follow '(',
            // and cannot follow another operator.
            if (prevType == 'S')
            {
                errorMessage = "Expression cannot start with an operator '" + string(1, ch) + "'.";
                return false;
            }
            if (prevType == '(')
            {
                errorMessage = "Operator '" + string(1, ch) + "' cannot immediately follow '('.";
                return false;
            }
            if (prevType == 'P')
            {
                errorMessage = "Two operators appear together (e.g. \"A++B\").";
                return false;
            }
            prevType = 'P';
        }
        else if (ch == '(')
        {
            // '(' cannot immediately follow an operand or ')' (no implicit multiply)
            if (prevType == 'O' || prevType == ')')
            {
                errorMessage = "'(' cannot immediately follow an operand or ')'.";
                return false;
            }
            prevType = '(';
        }
        else if (ch == ')')
        {
            // ')' cannot follow '(' (empty parentheses) or an operator, or be first
            if (prevType == '(')
            {
                errorMessage = "Empty parentheses '()' are not allowed.";
                return false;
            }
            if (prevType == 'P')
            {
                errorMessage = "Operator '" + rhs.substr(i > 0 ? i - 1 : 0, 1) + "' cannot appear right before ')'.";
                return false;
            }
            if (prevType == 'S')
            {
                errorMessage = "Expression cannot start with ')'.";
                return false;
            }
            prevType = ')';
        }
    }

    // ---- Check 8: expression must not END with an operator or '(' ----
    if (prevType == 'P')
    {
        errorMessage = "Expression cannot end with an operator.";
        return false;
    }
    if (prevType == '(')
    {
        errorMessage = "Expression cannot end with an unmatched '('.";
        return false;
    }

    // If we reach here, every check passed.
    return true;
}
