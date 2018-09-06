#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <string>

#include <lemon/list_graph.h>
#include <lemon/lgf_writer.h>

using namespace std;
using namespace lemon;

ListGraph g;
vector< ListGraph::Node > nodes;

void addNodes(int n, float p, float s) {
  srand(time(NULL));
  std::default_random_engine generator;
  normal_distribution<float> distribution(p, s);

  for (int i = 0; i < n; i++) {
    ListGraph::Node a = g.addNode();
    int numConnect = nodes.size()*distribution(generator);
    random_shuffle(nodes.begin(), nodes.end());
    int connectCount = 0;
    for (vector< ListGraph::Node >::iterator i = nodes.begin(); i != nodes.end() && connectCount!= numConnect; ++i) {
      g.addEdge(a, *i);
      connectCount++;
    }
    nodes.push_back(a);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    cout << "Usage: <executable> <numberOfNodes> <relativeProbabilityOfEdge> <relativeStandardDeviation>" << endl;
    exit(1);
  }
  srand(time(NULL));

  int n = atoi(argv[1]);
  float p = atof(argv[2]);
  float s = atof(argv[3]);

  addNodes(n, p, s);

  graphWriter(g, cout).run();

  return 0;
}
