// =======================================================================
// main.cpp
// -----------------------------------------------------------------------
// Entry point of the Instruction Set Simulator (ISS).
// Reads an arithmetic assignment expression, validates it, converts it
// to postfix, then lets the user generate 3/2/1/0-address instructions
// through a simple menu, looping until the user chooses Exit.
// =======================================================================

#include <iostream>
#include <string>
#include <vector>
#include <limits>     // for numeric_limits, used to safely clear bad input
#include <sstream>

#include "Utilities.h"
#include "Parser.h"
#include "Generator.h"

using namespace std;

// ---- Function prototypes for helper/display functions defined below ----
void printWelcomeAndRules();
void printMenu();
void printInstructions(const vector<string> &instructionList, const string &formatName);
bool readMenuChoice(int &choiceOut);

int main()
{
    printWelcomeAndRules();

    string rawExpression;
    string cleanExpression;
    string errorMessage;
    bool isValid = false;

    // -------------------------------------------------------------
    // Keep asking for an expression until a VALID one is entered.
    // This avoids the program simply quitting on the first typo.
    // -------------------------------------------------------------
    while (!isValid)
    {
        cout << "\nEnter your arithmetic expression: ";
        getline(cin, rawExpression);

        cleanExpression = removeSpaces(rawExpression);
        isValid = validateExpression(cleanExpression, errorMessage);

        if (!isValid)
        {
            cout << "ERROR: " << errorMessage << endl;
            cout << "Please try again.\n";
        }
    }

    cout << "\nExpression accepted: " << cleanExpression << endl;

    // Split "A=B+C" into lhs="A" and rhs="B+C"
    string lhs, rhs;
    splitExpression(cleanExpression, lhs, rhs);

    // Convert the right-hand side into postfix form using a stack.
    string postfixExpression = infixToPostfix(rhs);
    cout << "Postfix form of expression: " << postfixExpression << endl;

    int userChoice = 0;

    do
    {
        printMenu();
        cout << "Enter your choice (1-6): ";

        // readMenuChoice() safely reads an integer, and returns false
        // if the user typed something that is not a number at all.
        if (!readMenuChoice(userChoice))
        {
            cout << "\nInvalid input! Please enter a number between 1 and 6." << endl;
            continue;   // go back to the top of the loop and show the menu again
        }

        if (userChoice == 1)
        {
            vector<string> instructions = generateThreeAddressCode(postfixExpression, lhs);
            printInstructions(instructions, "THREE ADDRESS CODE");
        }
        else if (userChoice == 2)
        {
            vector<string> instructions = generateTwoAddressCode(postfixExpression, lhs);
            printInstructions(instructions, "TWO ADDRESS CODE");
        }
        else if (userChoice == 3)
        {
            vector<string> instructions = generateOneAddressCode(postfixExpression, lhs);
            printInstructions(instructions, "ONE ADDRESS CODE");
        }
        else if (userChoice == 4)
        {
            vector<string> instructions = generateZeroAddressCode(postfixExpression, lhs);
            printInstructions(instructions, "ZERO ADDRESS CODE");
        }
        else if (userChoice == 5)
        {
            vector<string> threeAddr = generateThreeAddressCode(postfixExpression, lhs);
            printInstructions(threeAddr, "THREE ADDRESS CODE");

            vector<string> twoAddr = generateTwoAddressCode(postfixExpression, lhs);
            printInstructions(twoAddr, "TWO ADDRESS CODE");

            vector<string> oneAddr = generateOneAddressCode(postfixExpression, lhs);
            printInstructions(oneAddr, "ONE ADDRESS CODE");

            vector<string> zeroAddr = generateZeroAddressCode(postfixExpression, lhs);
            printInstructions(zeroAddr, "ZERO ADDRESS CODE");
        }
        else if (userChoice == 6)
        {
            cout << "\nExiting program. Thank you!" << endl;
        }
        else
        {
            cout << "\nInvalid choice! Please enter a number between 1 and 6." << endl;
        }

    } while (userChoice != 6);

    return 0;
}

// =======================================================================
// FUNCTION DEFINITIONS
// =======================================================================

void printWelcomeAndRules()
{
    cout << "=========================================================" << endl;
    cout << "   INSTRUCTION SET SIMULATOR (ISS) FOR ARITHMETIC EXPR.   " << endl;
    cout << "   (0, 1, 2, 3 Address Instruction Formats)               " << endl;
    cout << "=========================================================" << endl;
    cout << "\nRULES for entering the expression:" << endl;
    cout << "  1. The expression MUST be an assignment, e.g. A=B+C     " << endl;
    cout << "  2. Only ONE '=' sign is allowed.                        " << endl;
    cout << "  3. Left of '=' must be a SINGLE uppercase letter (A-Z).  " << endl;
    cout << "  4. Operands (right side) are SINGLE uppercase letters.   " << endl;
    cout << "  5. Allowed operators are: + - * /                      " << endl;
    cout << "  6. Parentheses ( and ) are allowed and must be balanced. " << endl;
    cout << "  7. No numbers, lowercase letters, or other symbols.      " << endl;
    cout << "  8. Spaces are allowed and will be removed automatically. " << endl;
    cout << "\nExample valid input: A=(B+C)*D-E/F" << endl;
}

void printMenu()
{
    cout << "\n---------------------------------------------" << endl;
    cout << "                    MENU                      " << endl;
    cout << "---------------------------------------------" << endl;
    cout << "  1. Generate 3 Address Code" << endl;
    cout << "  2. Generate 2 Address Code" << endl;
    cout << "  3. Generate 1 Address Code" << endl;
    cout << "  4. Generate 0 Address Code" << endl;
    cout << "  5. Generate All Formats" << endl;
    cout << "  6. Exit" << endl;
    cout << "---------------------------------------------" << endl;
}

void printInstructions(const vector<string> &instructionList, const string &formatName)
{
    cout << "\n----- " << formatName << " -----" << endl;
    for (unsigned int i = 0; i < instructionList.size(); i++)
    {
        cout << instructionList[i] << endl;
    }
    cout << "Total Instructions: " << instructionList.size() << endl;
}

// -----------------------------------------------------------------------
// readMenuChoice()
// Safely reads an integer from cin. If the user types letters or
// symbols instead of a number, cin would normally enter a "failed"
// state and loop forever - this function detects that, CLEARS the
// error, discards the bad input, and returns false so the caller can
// show a friendly error message instead of crashing or looping badly.
// -----------------------------------------------------------------------
bool readMenuChoice(int &choiceOut)
{
    // Read the entire input line and parse it as an integer. This
    // avoids mixing formatted input (operator>>) with getline(),
    // which can leave leftover newlines in the stream.
    string line;
    if (!getline(cin, line))
    {
        return false;
    }

    // Use a stringstream to parse an integer and ensure the line
    // contains only a single number (no trailing garbage).
    stringstream ss(line);
    ss >> choiceOut;
    if (ss.fail())
    {
        return false;
    }
    // If there's any non-whitespace left after the number, treat as invalid.
    char leftover;
    if (ss >> leftover)
    {
        return false;
    }

    return true;
}
