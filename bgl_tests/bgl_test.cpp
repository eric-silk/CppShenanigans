#include <iostream>
#include <string>

#include "boost/graph/adjacency_list.hpp"

struct Vertex {
  std::string name;
}

struct Edge {
  std::string name;
}

class MyGraph {
public:
  MyGraph() = default;
  using Graph = boost::adjacency_list<, , , std::shared_ptr<Vertex>,
                                      std::shared_ptr<Edge>>;
  using VertexDesc = boost::graph_traits<Graph>::vertex_descriptor;
  using EdgeDesc = boost::graph_traits<Graph>::edge_descriptor;
}

int main() {
  // Create a n adjacency list, add some vertices.
  constexpr size_t num_tasks = 7;
  boost::adjacency_list<> g(num_tasks);
  // Add edges between vertices.
  boost::add_edge(0, 3, g);
  boost::add_edge(1, 3, g);
  boost::add_edge(1, 4, g);
  boost::add_edge(2, 1, g);
  boost::add_edge(3, 5, g);
  boost::add_edge(4, 6, g);
  boost::add_edge(5, 6, g);

  // Print the results.
  for (auto i : g) {
    std::cout << i << std::endl;
  }

  return 0;
}
