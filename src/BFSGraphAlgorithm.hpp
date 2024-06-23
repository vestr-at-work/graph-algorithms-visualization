#ifndef BFSGraphAlgorithm_hpp
#define BFSGraphAlgorithm_hpp

#include "GraphAlgorithm.hpp"
#include "PathGraph.hpp"
#include <queue>

// Class for Breadth-First Search (BFS) algorithm in a graph
class BFSGraphAlgorithm : public GraphAlgorithm {
    PathGraph m_graph;
    std::queue<std::size_t> m_unvisited_node_queue; 
    bool m_first_step;
    bool m_found_end;

public:
    /**
     * Constructor for BFSGraphAlgorithm.
     *
     * @param graph The graph to be traversed using BFS.
     */
    BFSGraphAlgorithm(PathGraph&& graph);

    /**
     * Executes the next step of the BFS algorithm.
     *
     * @return True if there are more steps to be taken, false otherwise.
     */
    virtual bool nextStep() override;

    /**
     * Returns the current state of the graph.
     *
     * @return A constant reference to the current state of the graph.
     */
    virtual const Graph& getState() const override;
};

#endif