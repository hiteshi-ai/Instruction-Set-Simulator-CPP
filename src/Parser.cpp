// =======================================================================
// Parser.cpp
// -----------------------------------------------------------------------
// Definitions for splitExpression() and infixToPostfix()
// =======================================================================

#include "Parser.h"
#include "Utilities.h"   // for isOperand(), isOperatorChar(), getPrecedence()
#include <stack>

// -----------------------------------------------------------------------
// splitExpression()
// Finds the '=' character and splits the string around it.
// -----------------------------------------------------------------------
void splitExpression(const string &expr, string &lhs, string &rhs)
{
    // find() returns the index of the first '=' character
    size_t equalsPos = expr.find('=');

    lhs = expr.substr(0, equalsPos);        // everything before '='
    rhs = expr.substr(equalsPos + 1);       // everything after '='
}

// -----------------------------------------------------------------------
// infixToPostfix()
// Standard stack-based algorithm to convert infix -> postfix.
//
// Rules used:
//   - If the character is an operand (A-Z), append it directly to output.
//   - If the character is '(', push it onto the stack.
//   - If the character is ')', pop from the stack and append to output
//     until the matching '(' is found; then discard that '('.
//   - If the character is an operator, pop and append any operators on
//     top of the stack that have GREATER OR EQUAL precedence, then push
//     the current operator.
//   - After scanning the whole string, pop all remaining operators from
//     the stack and append them to the output.
// -----------------------------------------------------------------------
string infixToPostfix(const string &infix)
{
    string postfix = "";        // the result (postfix expression)
    stack<char> operatorStack;  // holds operators and '(' temporarily

    for (unsigned int i = 0; i < infix.size(); i++)
    {
        char ch = infix[i];

        if (isOperand(ch))
        {
            // Operands go straight to the output.
            postfix += ch;
        }
        else if (ch == '(')
        {
            // Opening bracket: always pushed, acts as a "wall" marker.
            operatorStack.push(ch);
        }
        else if (ch == ')')
        {
            // Closing bracket: pop everything until we hit the matching '('.
            while (!operatorStack.empty() && operatorStack.top() != '(')
            {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            // Discard the '(' itself (do not add it to the output).
            if (!operatorStack.empty())
            {
                operatorStack.pop();
            }
        }
        else if (isOperatorChar(ch))
        {
            // Pop operators with higher-or-equal precedence before pushing
            // the current one (this keeps postfix evaluation order correct
            // and enforces left-to-right associativity).
            while (!operatorStack.empty() &&
                   operatorStack.top() != '(' &&
                   getPrecedence(operatorStack.top()) >= getPrecedence(ch))
            {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.push(ch);
        }
        // Any other character should not appear here because the
        // expression was already validated before this function is called.
    }

    // Pop any operators that are still left on the stack.
    while (!operatorStack.empty())
    {
        postfix += operatorStack.top();
        operatorStack.pop();
    }

    return postfix;
}
