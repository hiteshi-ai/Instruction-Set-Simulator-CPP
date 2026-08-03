# Instruction Set Simulator (C++)

A C++ based Instruction Set Simulator that converts arithmetic expressions into different instruction formats used in Computer Organization and Compiler Design.

The simulator accepts an arithmetic expression in infix notation, validates it, converts it into postfix notation, and generates:

- Three Address Code
- Two Address Code
- One Address Code
- Zero Address Code

---

## Project Overview

Instruction Set Simulator (ISS) is a console-based C++ application developed using modular programming. The project demonstrates how arithmetic expressions are translated into different instruction formats used by processors and compilers.

It uses stacks, vectors, strings, and expression parsing techniques to generate efficient instruction sequences.

---

## Features

✔ Accepts arithmetic expressions such as:

```
A=(B+C)*D-E/F
```

✔ Validates the expression before processing.

✔ Removes unnecessary spaces automatically.

✔ Converts Infix expression to Postfix.

✔ Generates:

- 3 Address Code
- 2 Address Code
- 1 Address Code
- 0 Address Code

✔ Displays the total number of instructions generated.

✔ Menu-driven console interface.

---

## Technologies Used

- C++
- Standard Template Library (STL)
- Stack
- Vector
- String
- Expression Parsing
- Infix to Postfix Conversion

---

## Folder Structure

```
Instruction-Set-Simulator-CPP/
│
├── src/
│   ├── main.cpp
│   ├── Utilities.cpp
│   ├── Utilities.h
│   ├── Parser.cpp
│   ├── Parser.h
│   ├── Generator.cpp
│   └── Generator.h
│
├── bin/
│   └── InstructionSetSimulator.exe
│
├──screenshotsprogram_screenshot2.png
│──screenshotsprogram_screenshot3.png
│   
│   
│
│
├── README.md
├── LICENSE
└── .gitignore
```

---

## Project Structure

### main.cpp

This is the entry point of the program. It interacts with the user, accepts the arithmetic expression, displays the menu, and calls the required functions to generate different instruction formats.

---

### Utilities.h / Utilities.cpp

These files contain helper functions used throughout the project.

Responsibilities:

- Remove unnecessary spaces
- Validate expressions
- Check brackets
- Check operators
- Verify assignment format

---

### Parser.h / Parser.cpp

These files process the arithmetic expression.

Responsibilities:

- Split the expression into Left Hand Side and Right Hand Side
- Convert Infix expression into Postfix using Stack

---

### Generator.h / Generator.cpp

These files generate the instruction formats.

Responsibilities:

- Generate Three Address Code
- Generate Two Address Code
- Generate One Address Code
- Generate Zero Address Code

---

## Algorithm

1. Read arithmetic expression.
2. Remove unnecessary spaces.
3. Validate the expression.
4. Split the expression into LHS and RHS.
5. Convert RHS from Infix to Postfix.
6. Generate the selected instruction format.
7. Display the generated instructions.
8. Repeat until the user exits.

---

## Sample Input

```
A=(B+C)*D-E/F
```

---

## Sample Output

### Three Address Code

```
T1 = B + C
T2 = T1 * D
T3 = E / F
T4 = T2 - T3
A = T4
```

---

### Two Address Code

```
MOV R1,B
ADD R1,C
MUL R1,D
MOV R2,E
DIV R2,F
SUB R1,R2
MOV A,R1
```

---

### One Address Code

```
LOAD B
ADD C
MUL D
STORE TEMP1
LOAD E
DIV F
STORE TEMP2
LOAD TEMP1
SUB TEMP2
STORE A
```

---

### Zero Address Code

```
PUSH B
PUSH C
ADD
PUSH D
MUL
PUSH E
PUSH F
DIV
SUB
POP A
```

---

## How to Compile

### Windows (MinGW)

```bash
g++ src/main.cpp src/Utilities.cpp src/Parser.cpp src/Generator.cpp -o bin/InstructionSetSimulator.exe
```

---

## How to Run

### Windows

```
bin\InstructionSetSimulator.exe
```

### Linux / macOS

```
g++ src/main.cpp src/Utilities.cpp src/Parser.cpp src/Generator.cpp -o iss

./iss
```

---

## Screenshots

### Program Execution

![Program Screenshot 2](screenshots/program_screenshot2.png)

![Program Screenshot 3](screenshots/program_screenshot3.png)

---

## Future Improvements

- Support numeric operands.
- Support variables with multiple characters.
- Add exponent operator (^).
- Develop a graphical user interface (GUI).
- Optimize generated instruction sequences.
- Support file input and output.

---

## License

This project is released under the MIT License.

---

## Author

**Hiteshi**

B.E. Computer Science Engineering (Artificial Intelligence)

Chandigarh University
