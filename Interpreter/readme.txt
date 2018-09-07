Compile: g++ interpreter.cpp -o interpret

Run: ./interpret <c-lite file>

Note, there is no syntax error detection. The interpreter
assumes the program is syntactically correct.

The syntax is similar to C. The only variable data type is int.
Lines are terminated with a new line, not ";"

Programs are terminated with the "exit" keyword.

Supported structures:
  receiving input with "in" keyword
  "if" and "while"
  any number of lines
  any variable name starting with a character
  printing strings with "out" keyword with at most one \n
  five arithmetic operations (+ - * / %)
  six conditionals (==, !=, <, >, <=, >=)
  white space is ignored

Examples of the syntax are given in test.clite and guess.clite.

test.clite: comprised of four demonstrative sections
  line  1: Fibonacci section - demonstrates while
  line 17: Division section - demonstrates input
  line 28: Odd section - shows % and "if" inside of while
  line 41: Multiplication section - demonstrates nested while

guess.clite: real world program example. guesses a users
  positive number using exponentiation.
