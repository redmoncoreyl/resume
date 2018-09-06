#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/adaptors.h>
#include <lemon/connectivity.h>

using namespace lemon;
using namespace std;

ListGraph g;
vector< ListGraph::Node > nodes;
vector< set< ListGraph::Node > > colors;
ListGraph::NodeMap<int> innerDegree(g);

bool vectorSort (const set< ListGraph::Node > & a, const set< ListGraph::Node > & b) {
  return a.size() < b.size();
}

void createPermutation() {
  for (ListGraph::NodeIt n(g); n != INVALID; ++n) {
    nodes.push_back(n);
  }

  // shuffle the vector
  srand(time(0));
  random_shuffle(nodes.begin(), nodes.end());
}

void classifyNodes(int k) {
  // iterate over the nodes in the vector and...
  //   add them to the smallest color class where
  //   adding it will not make any of the nodes of that color class exceed degree k
  //   or add it to a new color class if this is not possible
  for (vector< ListGraph::Node >::iterator n = nodes.begin(); n != nodes.end(); ++n) {
    int foundC = 0;
    for (vector< set< ListGraph::Node > >::iterator c = colors.begin(); c != colors.end() && !foundC; ++c) {
      int inC = 1;
      int adjInC = 0;
      for (ListGraph::IncEdgeIt e(g, *n); e != INVALID && inC; ++e) {
        if (c->find(g.oppositeNode(*n, e)) != c->end()) {
          adjInC++;
          if (innerDegree[g.oppositeNode(*n, e)] == k) {
            inC = 0;
          }
        }
        if (adjInC > k) {
          inC = 0;
        }
      }
      if (inC) {
        foundC = 1;
        c->insert(*n);
        innerDegree[*n] = adjInC;
        for (ListGraph::IncEdgeIt e(g, *n); e != INVALID && inC; ++e) {
          if (c->find(g.oppositeNode(*n, e)) != c->end()) {
            innerDegree[g.oppositeNode(*n, e)]++;
          }
        }
      }
    }
    if (!foundC) {
      set< ListGraph::Node > s;
      s.insert(*n);
      colors.push_back(s);
    }
    sort(colors.begin(), colors.end(), vectorSort);
  }
}

int main(int argc, char *argv[])  {
  if (argc != 4) {
    cout << "Usage: <executable> <LGF graph file> <k> <number of iterations>" << endl;
    exit(1);
  }

  srand(time(0));

  int k = atoi(argv[2]);
  int nIter = atoi(argv[3]);

  // read graph from lgf
  graphReader(g, argv[1]).run();

  // add the nodes to a vector and shuffle
  createPermutation();

  classifyNodes(k);
  int bestSize = colors.size();
  vector< set< ListGraph::Node > > bestColors = colors;
  for (int i = 0; i < nIter; i++) {
    colors.clear();
    random_shuffle(nodes.begin(), nodes.end());
    // add the nodes to the color classes
    classifyNodes(k);
    if (colors.size() < bestSize) {
      bestSize = colors.size();
      bestColors = colors;
    }
  }

  cout << "# colors: " << bestSize << endl;

  ListGraph::NodeMap<int> nodeColor(g);
  int curC = 0;
  for (vector< set< ListGraph::Node > >::iterator c = colors.begin(); c != colors.end(); ++c) {
    for (set< ListGraph::Node >::iterator n = c->begin(); n != c->end(); ++n) {
      nodeColor[*n] = curC;
    }
    curC++;
  }

  graphWriter(g, std::cout).nodeMap("color", nodeColor).run();

  return 0;
}
