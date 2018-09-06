#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <map>
#include <vector>
#include <stack>
#include <sstream>
#include <cctype>

using namespace std;

map <string, int> vars;
vector <string> ins;
vector <string> insTokens;
map <int, int> sections;
int exitAddr;
stack <int> curSections;

void readInstructions(char *file) {
  ifstream fin(file);
  if (!fin.good()) exit(1);

  int addr = 0;
  int loc = 0;
  stringstream buffer;
  buffer << fin.rdbuf();
  string contents(buffer.str());
  
  stack <int> unfin;

  string curIns = "";
  do {
    curIns += contents[loc];
    loc++;
  } while (curIns[curIns.size()-1] != '\n');
  int nonwhite = curIns.find_first_not_of(" ");
  curIns = curIns.substr(nonwhite, curIns.size()-nonwhite);
  ins.push_back(curIns);
  while (curIns != "exit\n") {
    if (curIns.substr(0, 3) == "if ")
      unfin.push(addr);  
    if (curIns.substr(0, 6) == "while ")
      unfin.push(addr);
    if (curIns.substr(0, 4) == "end\n") {
      sections[unfin.top()] = addr;
      unfin.pop();
    }
    curIns = "";
    do {
      curIns += contents[loc];
      loc++;
    } while (curIns[curIns.size()-1] != '\n');
    int nonwhite = curIns.find_first_not_of(" ");
    curIns = curIns.substr(nonwhite, curIns.size()-nonwhite);
    ins.push_back(curIns);
    addr++;
  }
  exitAddr = addr;
}

void tokenize(int addr) {
  while (insTokens.size() > 0) insTokens.pop_back();
  istringstream iss(ins[addr]);
  string sub;
  iss >> sub;
  if (sub == "out") {
    insTokens.push_back(sub);
    insTokens.push_back(ins[addr].substr(4, ins[addr].size()-5));
  } else {
    while (iss)  {
      insTokens.push_back(sub);
      iss >> sub;
    }
  }
}

int executeInstruction(int addr) {
  int nextAddr = addr + 1;

  tokenize(addr);

  if (insTokens.size() != 0) {
    if (insTokens[0] == "if" || insTokens[0] == "while") {
      int cmpVal = 0;
      if (isalpha(insTokens[1][0])) {
        cmpVal += vars[insTokens[1]];
      } else {
        cmpVal += atoi(insTokens[1].c_str());
      }
      if (isalpha(insTokens[3][0])) {
        cmpVal -= vars[insTokens[3]];
      } else {
        cmpVal -= atoi(insTokens[3].c_str());
      }
      if ((insTokens[2] == "==" && cmpVal != 0) ||
          (insTokens[2] == "!=" && cmpVal == 0) ||
          (insTokens[2] == "<" && cmpVal >= 0) ||
          (insTokens[2] == ">" && cmpVal <= 0) ||
          (insTokens[2] == "<=" && cmpVal > 0) ||
          (insTokens[2] == ">=" && cmpVal < 0)) {
        nextAddr = sections[addr] + 1;
      } else {
        if (insTokens[0] == "while") {
          curSections.push(addr);
        }
      }
    } else if (insTokens[0] == "end") {
      if (!curSections.empty() && sections[curSections.top()] == addr) {
        nextAddr = curSections.top();
        curSections.pop();
      }
    } else if (insTokens[0] == "exit") {
      nextAddr = -1;
    } else if (insTokens[0] == "in") {
      cin >> vars[insTokens[1]];
    } else if (insTokens[0] == "out") {
      if (insTokens[1][0] == '"') {
        if (insTokens[1].substr(insTokens[1].size()-3, 2) == "\\n") {
          cout << insTokens[1].substr(1, insTokens[1].size()-4);
          cout << endl;
        } else {
          cout << insTokens[1].substr(1, insTokens[1].size()-2);
        }
      } else {
        cout << vars[insTokens[1]];
      }
    } else {
      if (isalpha(insTokens[2][0])) {
        vars[insTokens[0]] = vars[insTokens[2]];
      } else {
        vars[insTokens[0]] = atoi(insTokens[2].c_str());
      }
      if (insTokens.size() == 5) {
        if (insTokens[3] == "+") {
          if (isalpha(insTokens[4][0])) {
            vars[insTokens[0]] += vars[insTokens[4]];
          } else {
            vars[insTokens[0]] += atoi(insTokens[4].c_str());
          }
        } else if (insTokens[3] == "-") {
          if (isalpha(insTokens[4][0])) {
            vars[insTokens[0]] -= vars[insTokens[4]];
          } else {
            vars[insTokens[0]] -= atoi(insTokens[4].c_str());
          }
        } else if (insTokens[3] == "*") {
          if (isalpha(insTokens[4][0])) {
            vars[insTokens[0]] *= vars[insTokens[4]];
          } else {
            vars[insTokens[0]] *= atoi(insTokens[4].c_str());
          }
        } else if (insTokens[3] == "/") {
          if (isalpha(insTokens[4][0])) {
            vars[insTokens[0]] /= vars[insTokens[4]];
          } else {
            vars[insTokens[0]] /= atoi(insTokens[4].c_str());
          }
        } else if (insTokens[3] == "%") {
          if (isalpha(insTokens[4][0])) {
            vars[insTokens[0]] %= vars[insTokens[4]];
          } else {
            vars[insTokens[0]] %= atoi(insTokens[4].c_str());
          }
        }
      }
    }
  }

  return nextAddr;
}

int main(int argc, char **argv) {
  if (argc != 2) exit(1);

  readInstructions(argv[1]);

  int addr = 0;
  do {
    addr = executeInstruction(addr);
  } while (addr != -1);

  return 0;
}
