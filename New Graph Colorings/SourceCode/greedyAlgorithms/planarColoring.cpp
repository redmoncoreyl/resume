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

using namespace lemon;
using namespace std;

ListGraph g;
vector< ListGraph::Node > nodes;
vector< vector< ListGraph::Node > > colors;

bool vectorSort (const vector< ListGraph::Node > & a, const vector< ListGraph::Node > & b) {
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

void classifyNodes() {
  // iterate over the nodes in the vector and...
  //   add them to the smallest color class where
  //   adding it will not destroy acylcic
  //   or add it to a new color class
  for (vector< ListGraph::Node >::iterator n = nodes.begin(); n != nodes.end(); ++n) {
    int foundC = 0;
    for (vector< vector< ListGraph::Node > >::iterator c = colors.begin(); c != colors.end() && !foundC; ++c) {
      ListGraph::NodeMap<bool> filter(g, false);
      for (vector<ListGraph::Node>::iterator cn = c->begin(); cn != c->end(); ++cn) {
        filter[*cn] = true;
      }
      FilterNodes<ListGraph> subgraph(g, filter);
      subgraph.enable(*n);
      if (checkPlanarity(subgraph)) {
        c->push_back(*n);
        foundC = 1;
      } else {
        subgraph.disable(*n);
      }
    }
    if (!foundC) {
      vector< ListGraph::Node > v;
      v.push_back(*n);
      colors.push_back(v);
    }
    sort(colors.begin(), colors.end(), vectorSort);
  }
}

int main(int argc, char *argv[])  {
  if (argc != 3) {
    cout << "Usage: <executable> <LGF graph file> <number of iterations>" << endl;
    exit(1);
  }

  srand(time(0));

  int nIter = atoi(argv[2]);

  // read graph from lgf
  graphReader(g, argv[1]).run();

  // add the nodes to a vector and shuffle
  createPermutation();

  classifyNodes();
  int bestSize = colors.size();
  vector< vector< ListGraph::Node > > bestColors = colors;
  for (int i = 0; i < nIter; i++) {
    colors.clear();
    random_shuffle(nodes.begin(), nodes.end());
    // add the nodes to the color classes
    classifyNodes();
    if (colors.size() < bestSize) {
      bestSize = colors.size();
      bestColors = colors;
    }
  }

  cout << "# colors: " << bestSize << endl;

  ListGraph::NodeMap<int> nodeColor(g);
  int curC = 0;
  for (vector< vector< ListGraph::Node > >::iterator c = colors.begin(); c != colors.end(); ++c) {
    for (vector< ListGraph::Node >::iterator n = c->begin(); n != c->end(); ++n) {
      nodeColor[*n] = curC;
    }
    curC++;
  }

  graphWriter(g, std::cout).nodeMap("color", nodeColor).run();

  return 0;
}
