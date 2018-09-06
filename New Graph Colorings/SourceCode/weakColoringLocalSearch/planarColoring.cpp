#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/adaptors.h>
#include <lemon/planarity.h>

using namespace std;
using namespace lemon;

ListGraph g;
ListGraph::NodeMap<int> nodeColor(g);
vector< vector< ListGraph::Node > > colors;

bool vectorSort (const vector< ListGraph::Node > & a, const vector< ListGraph::Node > & b) {
  return a.size() > b.size();
}

void loadColors() {
  int maxClass = 0;
  for (ListGraph::NodeIt n(g); n != INVALID; ++n) {
    if (nodeColor[n] > maxClass) {
      maxClass = nodeColor[n];
    }
  }

  for (int i = 0; i <= maxClass; i++) {
    vector< ListGraph::Node > curNodes;
    for (ListGraph::NodeIt n(g); n != INVALID; ++n) {
      if (nodeColor[n] == i) {
        curNodes.push_back(n);
      }
    }
    if (curNodes.size() > 0) {
      colors.push_back(curNodes);
    }
  }
}

bool moveColor() {
  // pick the largest color class
  sort(colors.begin(), colors.end(), vectorSort);
  vector< vector< ListGraph::Node > >::iterator largestColor = colors.begin();

  // shuffle this class
  random_shuffle(largestColor->begin(), largestColor->end());

  // try to move the first node to any smaller class
  bool foundNode = false;
  vector< ListGraph::Node >::iterator movingNode = largestColor->begin();
  vector< vector< ListGraph::Node > >::iterator destinationClass;
  vector< ListGraph::Node >::iterator foundMovingNode = largestColor->begin();
  vector< vector< ListGraph::Node > >::iterator foundDestinationClass;
  for (destinationClass = colors.begin()+1; destinationClass != colors.end() && !foundNode; ++destinationClass) {
    ListGraph::NodeMap<bool> filter(g, false);
    vector< ListGraph::Node >::iterator classNode;
    for (classNode = destinationClass->begin(); classNode != destinationClass->end(); ++classNode) {
      filter[*classNode] = true;
    }
    filter[*movingNode] = true;
    FilterNodes<ListGraph> subgraph(g, filter);
    if (checkPlanarity(subgraph)) {
      foundNode = true;
      foundMovingNode = movingNode;
      foundDestinationClass = destinationClass;
    }
  }

  // if a node was found, move it
  // if it was not found, add a new color class
  if (foundNode) {
    foundDestinationClass->push_back(*foundMovingNode);
    largestColor->erase(foundMovingNode);
    if (largestColor->empty()) {
      colors.erase(largestColor);
    }
  } else {
    vector< ListGraph::Node > newClass;
    newClass.push_back(*foundMovingNode);
    largestColor->erase(foundMovingNode);
    if (largestColor->empty()) {
      colors.erase(largestColor);
    }
  }

  // only return false if every single class is planar
  for (vector< vector< ListGraph::Node > >::iterator classes = colors.begin(); classes != colors.end(); classes++) {
    ListGraph::NodeMap<bool> filter(g, false);
    vector< ListGraph::Node >::iterator classNode;
    for (classNode = classes->begin(); classNode != classes->end(); ++classNode) {
      filter[*classNode] = true;
    }
    FilterNodes<ListGraph> subgraph(g, filter);
    if (!checkPlanarity(subgraph)) {
      return true;
    }
  }
  return false;
}

void printColors() {
  cout << "NEW ROUND" << endl;
  for (int i = 0; i < colors.size(); i++) {
    for (int j = 0; j < colors[i].size(); j++) {
      cout << g.id(colors[i][j]) << " ";
    }
    cout << endl;
  }
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  if (argc != 2) {
    cout << "Usage: <executable> <LGF file with color classes>" << endl;
    exit(1);
  }

  graphReader(g, argv[1]).nodeMap("color", nodeColor).run();

  loadColors();
  while (moveColor()) {
  }

  cout << "# colors: " << colors.size() << endl;

  ListGraph::NodeMap<int> nodeColor2(g, 0);
  int curC = 0;
  for (vector< vector< ListGraph::Node > >::iterator c = colors.begin(); c != colors.end(); ++c) {
    for (vector< ListGraph::Node >::iterator n = c->begin(); n != c->end(); ++n) {
      nodeColor2[*n] = curC;
    }
    curC++;
  }

  graphWriter(g, std::cout).nodeMap("color", nodeColor2).run();

  return 0;
}
