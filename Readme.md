# Instruction Set Simulator (C++)

## Overview

This project is a C++ based Instruction Set Simulator that converts an arithmetic expression into different instruction formats.

It supports:

- 3 Address Instructions
- 2 Address Instructions
- 1 Address Instructions
- 0 Address Instructions

The simulator accepts arithmetic expressions containing uppercase operands (A-Z), operators (+, -, *, /), parentheses, and assignment (=).

## Features

- Validates user input
- Converts infix expression to postfix
- Generates Three Address Code
- Generates Two Address Code
- Generates One Address Code
- Generates Zero Address Code
- Displays total instructions generated

## Technologies Used

- C++
- Stack
- Vector
- String
- STL

## Folder Structure

Instruction-Set-Simulator-CPP/
│
├── src/
├── bin/
├── README.md
├── LICENSE
└── .gitignore

## How to Compile

Windows (MinGW)

```bash
g++ src/main.cpp src/Utilities.cpp src/Parser.cpp src/Generator.cpp -o bin/InstructionSetSimulator.exe
```

## How to Run

Windows

```bash
bin\InstructionSetSimulator.exe
```

---

## Sample Input

```
A=(B+C)*D-E/F
```

---

## Author

Hiteshi
B.Tech CSE (AI)
