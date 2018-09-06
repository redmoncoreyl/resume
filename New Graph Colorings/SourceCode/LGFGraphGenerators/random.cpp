#include <iostream>
#include <ctime>
#include <cstdlib>

#include <lemon/list_graph.h>
#include <lemon/lgf_writer.h>

using namespace std;
using namespace lemon;

ListGraph g;
vector< ListGraph::Node > nodes;
vector< ListGraph::Edge > edges;

void addNodes(int n) {
  for (int i = 0; i < n; i++) {
    ListGraph::Node a = g.addNode();
    nodes.push_back(a);
  }
}

void addEdges() {
  for (vector< ListGraph::Node >::iterator i = nodes.begin(); i != nodes.end(); ++i) {
    for (vector< ListGraph::Node >::iterator j = i + 1; j != nodes.end(); ++j) {
      ListGraph::Edge e = g.addEdge(*i, *j);
      edges.push_back(e);
    }
  }
}

void eraseEdges(float p) {
  for (vector< ListGraph::Edge >::iterator i = edges.begin(); i != edges.end(); ++i) {
    float r = (rand()/(float)RAND_MAX);
    if (r > p) {
      g.erase(*i);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "Usage: <executable> <numberOfNodes> <probabilityOfEdge>" << endl;
    exit(1);
  }
  srand(time(NULL));

  int n = atoi(argv[1]);
  float p = atof(argv[2]);

  addNodes(n);

  addEdges();

  eraseEdges(p);

  graphWriter(g, cout).run();

  return 0;
}
