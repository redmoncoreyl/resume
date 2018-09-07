Compile: g++ main.cpp -o turing

Run: ./turing < <turing machine>

Turing machine files contain four sections: transitions,
accepting, rejecting, and input (in that order). Transitions
have the following syntax:
  qm:X/Y,D-qn where δ(qm, X) = (qn, Y, D)

Several example files have been included:
  anbncn.txt: tests if input belongs to AnBnCn, ∑ = {a, b, c}
  neverhalt.txt: simple infinite loop machine
  palindromes.txt: tests if input is a palendrome, ∑ = {a, b}
  reverse.txt: reverses input, ∑ = {a, b}
  strcpy.txt: copies input, ∑ = {a, b}
  xRx.txt: if input = x, output = x Reverse(x), ∑ = {a, b}
  xx.txt: tests if input belongs to xx, ∑ = {a, b}
