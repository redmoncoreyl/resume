#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/adaptors.h>
#include <lemon/connectivity.h>

using namespace std;
using namespace lemon;

ListGraph g;
ListGraph::NodeMap<int> nodeColor(g);
vector< set< ListGraph::Node > > colors;
ListGraph::NodeMap<int> innerDegree(g);

bool vectorSort (const set< ListGraph::Node > & a, const set< ListGraph::Node > & b) {
  return a.size() < b.size();
}

void loadColors() {
  int maxClass = 0;
  for (ListGraph::NodeIt n(g); n != INVALID; ++n) {
    if (nodeColor[n] > maxClass) {
      maxClass = nodeColor[n];
    }
  }

  for (int i = 0; i <= maxClass; i++) {
    set< ListGraph::Node > curNodes;
    for (ListGraph::NodeIt n(g); n != INVALID; ++n) {
      if (nodeColor[n] == i) {
        curNodes.insert(n);
      }
    }
    if (curNodes.size() > 0) {
      colors.push_back(curNodes);
    }
  }

  for(vector< set< ListGraph::Node > >::iterator c = colors.begin(); c != colors.end(); ++c) {
    for (set< ListGraph::Node >::iterator n = c->begin(); n != c->end(); ++n) {
      int deg = 0;
      for (ListGraph::IncEdgeIt e(g, *n); e != INVALID; ++e) {
        if (c->find(g.oppositeNode(*n, e)) != c->end()) {
          deg++;
        }
      }
      innerDegree[*n] = deg;
    }
  }
}

bool moveColor(int k) {
  // pick the smallest color class
  sort(colors.begin(), colors.end(), vectorSort);
  vector< set< ListGraph::Node > >::iterator smallestColor = colors.begin();

  // // shuffle this class
  // random_shuffle(smallestColor->begin(), smallestColor->end());

  // try to move any of the nodes to any bigger class
  bool foundNode = false;
  set< ListGraph::Node >::iterator movingNode;
  vector< set< ListGraph::Node > >::iterator destinationClass;
  set< ListGraph::Node >::iterator foundMovingNode;
  vector< set< ListGraph::Node > >::iterator foundDestinationClass;
  for (movingNode = smallestColor->begin(); movingNode != smallestColor->end() && !foundNode; ++movingNode) {
    for (destinationClass = colors.begin()+1; destinationClass != colors.end() && !foundNode; ++destinationClass) {
      bool inC = true;
      int neighborsInDest = 0;
      for (ListGraph::IncEdgeIt e(g, *movingNode); e != INVALID && inC; ++e) {
        if (destinationClass->find(g.oppositeNode(*movingNode, e)) != destinationClass->end()) {
          neighborsInDest++;
          if (innerDegree[g.oppositeNode(*movingNode, e)] == k) {
            inC = false;
          }
        }
        if (neighborsInDest > k) {
          inC = false;
        }
      }
      if (inC) {
        foundNode = true;
        foundMovingNode = movingNode;
        foundDestinationClass = destinationClass;
      }
    }
  }

  // if a node was found, move it. otherwise return false
  if (foundNode) {
    int deg = 0;
    for (ListGraph::IncEdgeIt e(g, *foundMovingNode); e != INVALID; ++e) {
      if (foundDestinationClass->find(g.oppositeNode(*foundMovingNode, e)) != foundDestinationClass->end()) {
        innerDegree[g.oppositeNode(*foundMovingNode, e)]++;
        deg++;
      }
    }
    innerDegree[*foundMovingNode] = deg;
    foundDestinationClass->insert(*foundMovingNode);
    smallestColor->erase(foundMovingNode);
    if (smallestColor->empty()) {
      colors.erase(smallestColor);
    }
    return true;
  }
  return false;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  if (argc != 3) {
    cout << "Usage: <executable> <LGF file with color classes> <k>" << endl;
    exit(1);
  }
  int k = atoi(argv[2]);

  graphReader(g, argv[1]).nodeMap("color", nodeColor).run();

  loadColors();
  while (moveColor(k)) {
  }

  cout << "# colors: " << colors.size() << endl;

  ListGraph::NodeMap<int> nodeColor2(g, 0);
  int curC = 0;
  for (vector< set< ListGraph::Node > >::iterator c = colors.begin(); c != colors.end(); ++c) {
    for (set< ListGraph::Node >::iterator n = c->begin(); n != c->end(); ++n) {
      nodeColor2[*n] = curC;
    }
    curC++;
  }

  graphWriter(g, std::cout).nodeMap("color", nodeColor2).run();

  return 0;
}
