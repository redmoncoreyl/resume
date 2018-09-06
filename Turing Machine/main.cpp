#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <stdlib.h>

using namespace std;

struct transition {
  char replacement;
  char direction;
  int toStateID;
  transition(char r, char d, int s) {
    replacement = r; direction = d; toStateID = s;
  } 
};

struct state {
  int ID;
  map <char, transition> transitions;
  state(int i) {ID = i;}
};

vector <state> states;
vector <int> acceptingStates;
vector <int> rejectingStates;
map <int, int> stateLoc;

void readTransitions() {
  string curLine;
  int fromState;
  int toState;
  
  cin >> curLine;
  cin >> curLine;
  while (curLine != "accepting:") {
    fromState = atoi((curLine.substr(1, curLine.find(":") - 1)).c_str());
    toState = atoi((curLine.substr(curLine.rfind("q") + 1)).c_str());
    if (stateLoc.find(fromState) == stateLoc.end()) {
      states.push_back(state(fromState));
      stateLoc[fromState] = states.size() - 1;
    }
    if (stateLoc.find(toState) == stateLoc.end()) {
      states.push_back(state(toState));
      stateLoc[toState] = states.size() - 1;
    }
    
    char c = curLine.at(curLine.find(":") + 1);
    char r = curLine.at(curLine.find(":") + 3);
    char d = curLine.at(curLine.find(":") + 5);
    map <char, transition> *example = &states[stateLoc[fromState]].transitions;
    example->insert(pair <char, transition> (c, transition (r, d, toState)));
    
    cin >> curLine;
  }
}

void readAccepting() {
  string curLine;
  
  cin >> curLine;
  
  while (curLine != "rejecting:") {
    int SID = atoi((curLine.substr(1)).c_str());
    acceptingStates.push_back(SID);
    
    cin >> curLine;
  }
}

void readRejecting() {
  string curLine;
  
  cin >> curLine;
  
  while (curLine != "input:") {
    int SID = atoi((curLine.substr(1)).c_str());
    rejectingStates.push_back(SID);
    
    cin >> curLine;
  }
}

void printTape(vector <char> tape) {
  for (int i = 0; i < tape.size(); i++) {
    cout << tape[i];
  }
  cout << endl;
}

bool shouldHalt (int stateID, char tapeHead) {
  map <char, transition> myMap = states[stateLoc[stateID]].transitions;
  bool transitionNotExist = (myMap.find(tapeHead) == myMap.end());
  return (find(acceptingStates.begin(), acceptingStates.end(), stateID)
            != acceptingStates.end() ||
            find(rejectingStates.begin(), rejectingStates.end(), stateID)
            != rejectingStates.end() || transitionNotExist);
}

void readInput() {
  string input;
  
  cin >> input;
  
  vector <char> tape;
  
  tape.push_back('|');
  for (int i = 0; i < input.size(); i++) {
    tape.push_back(input.at(i));
  }
  for (int i = 0; i < 49 - input.size(); i++) {
    tape.push_back('|');
  }
  
  int head = 0;
  int curStateID = 0;
  
  printTape(tape);
  
  while (!shouldHalt(curStateID, tape[head])) {
    map <char, transition> myMap = states[stateLoc[curStateID]].transitions;
    char tapeHead = tape[head];
    tape[head] = myMap.at(tapeHead).replacement;
    if (myMap.at(tapeHead).direction == 'r') { head++; } else { head--; }
    curStateID = myMap.at(tapeHead).toStateID;
    
    if (tapeHead != myMap.at(tapeHead).replacement)
      printTape(tape);
  }
  
  if (find(acceptingStates.begin(), acceptingStates.end(), curStateID)
            != acceptingStates.end()) {
    cout << "Accepted\n";
  } else {
    cout << "Rejected\n";
  }
}

int main() {
  readTransitions();
  readAccepting();
  readRejecting();
  readInput();
  return 0;
}
