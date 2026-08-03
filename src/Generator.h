// =======================================================================
// Generator.h
// -----------------------------------------------------------------------
// Declares the four instruction-generator functions - one for each
// machine type (3-address, 2-address, 1-address, 0-address).
// Each function takes the POSTFIX expression and the result variable
// name (the left-hand side, e.g. "A"), and returns a vector of strings
// where each string is exactly one generated instruction line.
// Defined in Generator.cpp
// =======================================================================

#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>
#include <vector>
using namespace std;

// Three Address Machine: T1 = B + C   (uses temporary variables T1,T2,...)
vector<string> generateThreeAddressCode(const string &postfix, const string &resultVar);

// Two Address Machine: MOV R1,B / ADD R1,C ...  (uses registers R1,R2,...)
vector<string> generateTwoAddressCode(const string &postfix, const string &resultVar);

// One Address Machine: LOAD B / ADD C ...  (uses a single accumulator + TEMP memory)
vector<string> generateOneAddressCode(const string &postfix, const string &resultVar);

// Zero Address Machine: PUSH B / PUSH C / ADD ...  (uses an operand stack)
vector<string> generateZeroAddressCode(const string &postfix, const string &resultVar);

#endif // GENERATOR_H
