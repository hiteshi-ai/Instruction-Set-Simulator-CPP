// =======================================================================
// Generator.cpp
// -----------------------------------------------------------------------
// This file contains the actual logic that reads a POSTFIX expression
// and produces instructions for each of the four classic machine types
// taught in Computer Organization:
//
//   3-Address Machine : uses temporary variables (T1, T2, ...)
//   2-Address Machine : uses registers (R1, R2, ...)
//   1-Address Machine : uses a single Accumulator (ACC) + TEMP memory
//   0-Address Machine : uses an operand stack (PUSH/POP)
//
// All four functions take the postfix string (e.g. "BC+D*EF/-") and the
// result variable name (e.g. "A"), and return a vector<string> where
// each element is ONE line of generated assembly-like code.
// =======================================================================

#include "Generator.h"
#include "Utilities.h"   // for isOperand(), isOperatorChar()
#include <vector>
#include <string>
#include <sstream>   // for converting int -> string via ostringstream

// -----------------------------------------------------------------------
// intToString()
// A small local helper that converts an integer (like a counter) into
// a string, e.g. 1 -> "1". We use ostringstream instead of std::to_string
// purely out of habit for portability, but to_string works fine too.
// -----------------------------------------------------------------------
static string intToString(int number)
{
    ostringstream oss;
    oss << number;
    return oss.str();
}

// -----------------------------------------------------------------------
// getMnemonic()
// Converts an operator character into its instruction mnemonic word.
// Example: '+' -> "ADD", '-' -> "SUB", '*' -> "MUL", '/' -> "DIV"
// -----------------------------------------------------------------------
static string getMnemonic(char op)
{
    if (op == '+') return "ADD";
    if (op == '-') return "SUB";
    if (op == '*') return "MUL";
    return "DIV";   // the only remaining possibility is '/'
}

// =======================================================================
// 1) THREE ADDRESS CODE GENERATOR
// -----------------------------------------------------------------------
// Uses a stack of operand NAMES (either a plain variable like "B" or a
// temporary like "T1"). Each operator pops the top two names, creates a
// brand-new temporary to hold the result, and prints one instruction:
//      Tn = left  OP  right
// The new temporary is pushed back so it can be used in later operations.
// =======================================================================
vector<string> generateThreeAddressCode(const string &postfix, const string &resultVar)
{
    vector<string> instructions;   // the output instruction list
    vector<string> nameStack;      // simple stack implemented with a vector
    int tempCounter = 0;           // counts T1, T2, T3, ...

    for (unsigned int i = 0; i < postfix.size(); i++)
    {
        char token = postfix[i];

        if (isOperand(token))
        {
            // Operand: push its single-letter name onto the stack.
            nameStack.push_back(string(1, token));
        }
        else if (isOperatorChar(token))
        {
            // Operator: pop the two most recent operands.
            // In postfix, the RIGHT operand was pushed last, so it comes
            // off the stack FIRST.
            string rightOperand = nameStack.back(); nameStack.pop_back();
            string leftOperand  = nameStack.back(); nameStack.pop_back();

            // Create a new temporary variable name: T1, T2, T3 ...
            tempCounter++;
            string tempName = "T" + intToString(tempCounter);

            // Build and store the instruction text.
            string instruction = tempName + " = " + leftOperand + " " + token + " " + rightOperand;
            instructions.push_back(instruction);

            // The new temporary now represents this sub-expression's value.
            nameStack.push_back(tempName);
        }
        // Any other character cannot appear here since postfix only
        // ever contains operands and the four operators.
    }

    // After the loop, exactly one name remains on the stack: the final result.
    string finalResult = nameStack.back();
    nameStack.pop_back();

    // Final instruction: assign the result into the destination variable.
    instructions.push_back(resultVar + " = " + finalResult);

    return instructions;
}

// =======================================================================
// 2) TWO ADDRESS CODE GENERATOR
// -----------------------------------------------------------------------
// Uses REGISTERS (R1, R2, ...). Every stack item remembers both a NAME
// and whether that name is a register or a plain variable.
//
// Key idea: a two-address instruction can freely name TWO locations
// ("OP dest,src"), so unlike the 1-address machine, no location ever
// needs to be "rescued" into a temporary before use - a register is
// always a valid, addressable location.
//
// Rule for each operator:
//   - pop right operand, pop left operand
//   - if the LEFT operand is already sitting in a register, reuse that
//     SAME register in-place:      OP  Rx , right
//   - otherwise, allocate a brand new register, move the left operand
//     into it, then operate:       MOV Rx , left
//                                  OP  Rx , right
//   - push the register that now holds the result back onto the stack
// At the very end, MOV the final value into the destination variable.
// =======================================================================

// A tiny helper structure to remember a stack item's name AND whether
// that name refers to a register (true) or a plain variable (false).
struct StackItem2Addr
{
    string name;
    bool isRegister;
};

vector<string> generateTwoAddressCode(const string &postfix, const string &resultVar)
{
    vector<string> instructions;
    vector<StackItem2Addr> itemStack;
    int registerCounter = 0;

    for (unsigned int i = 0; i < postfix.size(); i++)
    {
        char token = postfix[i];

        if (isOperand(token))
        {
            // Plain variable: not (yet) in a register.
            StackItem2Addr item;
            item.name = string(1, token);
            item.isRegister = false;
            itemStack.push_back(item);
        }
        else if (isOperatorChar(token))
        {
            // Pop right operand (pushed last) then left operand.
            StackItem2Addr rightOperand = itemStack.back(); itemStack.pop_back();
            StackItem2Addr leftOperand  = itemStack.back(); itemStack.pop_back();

            string mnemonic = getMnemonic(token);
            StackItem2Addr resultItem;

            if (leftOperand.isRegister)
            {
                // Left operand already lives in a register -> reuse it directly.
                instructions.push_back(mnemonic + " " + leftOperand.name + "," + rightOperand.name);
                resultItem.name = leftOperand.name;
            }
            else
            {
                // Need a fresh register to hold the left operand first.
                registerCounter++;
                string newRegister = "R" + intToString(registerCounter);
                instructions.push_back("MOV " + newRegister + "," + leftOperand.name);
                instructions.push_back(mnemonic + " " + newRegister + "," + rightOperand.name);
                resultItem.name = newRegister;
            }

            resultItem.isRegister = true;   // the result always ends up in a register
            itemStack.push_back(resultItem);
        }
    }

    // Whatever remains on the stack is the final computed value.
    StackItem2Addr finalItem = itemStack.back();
    itemStack.pop_back();

    // Move the final value (register or plain variable) into the destination.
    instructions.push_back("MOV " + resultVar + "," + finalItem.name);

    return instructions;
}

// =======================================================================
// 3) ONE ADDRESS CODE GENERATOR (Accumulator machine)
// -----------------------------------------------------------------------
// This machine has exactly ONE working register: the Accumulator (ACC).
// Every instruction is either:
//    LOAD  x     -> ACC = x
//    STORE x     -> x = ACC
//    ADD/SUB/MUL/DIV x  -> ACC = ACC (op) x
//
// Because there is only one accumulator, whenever we compute a value
// that is needed LATER (not immediately consumed by the very next
// operation) we must STORE it into a temporary memory location first,
// otherwise a later LOAD would silently overwrite and lose it.
//
// We track this with a stack of items, where each item is either:
//   - a plain named location (a variable like "B" or a temp like "TEMP1"), or
//   - a special "pending" marker meaning "this value currently sits in
//     the accumulator and has NOT been stored anywhere yet".
// =======================================================================

struct StackItem1Addr
{
    string name;        // variable / temp name (empty if isAccPending)
    bool isAccPending;   // true = "this is the value currently in ACC, unsaved"
};

vector<string> generateOneAddressCode(const string &postfix, const string &resultVar)
{
    vector<string> instructions;
    vector<StackItem1Addr> itemStack;
    int tempCounter = 0;
    bool accDirty = false;   // true if ACC holds a value that is not yet stored

    for (unsigned int i = 0; i < postfix.size(); i++)
    {
        char token = postfix[i];

        if (isOperand(token))
        {
            // Plain operand: just remember its name, it lives in memory already.
            StackItem1Addr item;
            item.name = string(1, token);
            item.isAccPending = false;
            itemStack.push_back(item);
        }
        else if (isOperatorChar(token))
        {
            // Pop right operand (op2) then left operand (op1).
            StackItem1Addr op2 = itemStack.back(); itemStack.pop_back();
            StackItem1Addr op1 = itemStack.back(); itemStack.pop_back();

            string mnemonic = getMnemonic(token);

            // --- Rule A -------------------------------------------------
            // A one-address instruction's second argument MUST be a named
            // memory location - it can never be "whatever is currently in
            // ACC". So if op2 itself is the live ACC value, save it first.
            if (op2.isAccPending)
            {
                tempCounter++;
                string tempName = "TEMP" + intToString(tempCounter);
                instructions.push_back("STORE " + tempName);
                op2.name = tempName;
                op2.isAccPending = false;
                accDirty = false;
            }

            // --- Rule B -------------------------------------------------
            // We are about to LOAD op1 into ACC (unless op1 is already the
            // live ACC value). If ACC currently holds some OTHER pending,
            // unstored value that is still needed later, save it now -
            // otherwise the upcoming LOAD would destroy it.
            if (!op1.isAccPending && accDirty)
            {
                tempCounter++;
                string tempName = "TEMP" + intToString(tempCounter);
                instructions.push_back("STORE " + tempName);
                // Find the still-pending item in the stack and rename it,
                // since its value is now safely stored under 'tempName'.
                for (int k = static_cast<int>(itemStack.size()) - 1; k >= 0; k--)
                {
                    if (itemStack[k].isAccPending)
                    {
                        itemStack[k].name = tempName;
                        itemStack[k].isAccPending = false;
                        break;
                    }
                }
                accDirty = false;
            }

            // --- Emit the LOAD (if needed) and the operation ------------
            if (op1.isAccPending)
            {
                // op1's value is still sitting in ACC from the previous
                // step - no LOAD needed, operate directly.
                instructions.push_back(mnemonic + " " + op2.name);
            }
            else
            {
                instructions.push_back("LOAD " + op1.name);
                instructions.push_back(mnemonic + " " + op2.name);
            }

            // The result of this operation now lives in ACC, unsaved.
            accDirty = true;
            StackItem1Addr resultItem;
            resultItem.name = "";
            resultItem.isAccPending = true;
            itemStack.push_back(resultItem);
        }
    }

    // After the loop, exactly one item remains: the final result.
    StackItem1Addr finalItem = itemStack.back();
    itemStack.pop_back();

    // If the final result is not already sitting in ACC (e.g. a plain
    // assignment like A=B with no operators at all), load it first.
    if (!finalItem.isAccPending)
    {
        instructions.push_back("LOAD " + finalItem.name);
    }

    // Store the final accumulator value into the destination variable.
    instructions.push_back("STORE " + resultVar);

    return instructions;
}

// =======================================================================
// 4) ZERO ADDRESS CODE GENERATOR (Stack machine)
// -----------------------------------------------------------------------
// The simplest of the four. Every operand becomes a PUSH, and every
// operator simply pops the top two stack values, combines them and
// pushes the result back - which maps DIRECTLY onto the postfix string
// itself; no extra bookkeeping is required at all.
// =======================================================================
vector<string> generateZeroAddressCode(const string &postfix, const string &resultVar)
{
    vector<string> instructions;

    for (unsigned int i = 0; i < postfix.size(); i++)
    {
        char token = postfix[i];

        if (isOperand(token))
        {
            instructions.push_back("PUSH " + string(1, token));
        }
        else if (isOperatorChar(token))
        {
            instructions.push_back(getMnemonic(token));
        }
    }

    // The final value left on the stack is popped into the destination.
    instructions.push_back("POP " + resultVar);

    return instructions;
}
