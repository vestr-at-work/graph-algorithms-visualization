#ifndef FordFulkersonGraphAlgorithm_hpp
#define FordFulkersonGraphAlgorithm_hpp

#include "FlowGraph.hpp"
#include "GraphAlgorithm.hpp"

// Class for Ford-Fulkerson algorithm in a flow graph
class FordFulkersonGraphAlgorithm : public GraphAlgorithm {
    enum class NextStepState { CleanGraph, PathFind, UpdatePath };

    FlowGraph m_graph;
    std::size_t m_start_node; // Index of the start node
    std::size_t m_end_node; // Index of the end node
    std::vector<std::size_t> m_parent; // For trasking parent nodes during BFS
    std::size_t m_max_flow;
    NextStepState m_next_step_state; // Tells nextStep which part of the algorithm should be done next

    /**
     * Performs Breadth-First Search (BFS) to find an augmenting path in the flow graph.
     *
     * @return True if an augmenting path is found, false otherwise.
     */
    bool BFS();

    /**
     * Finds the bottleneck capacity along an augmenting path and updates the flow.
     *
     * @return The maximum flow in the path.
     */
    std::size_t updateFlow();

    /**
     * Helper function to find the flow edge between two nodes.
     *
     * @param from_node Index of the starting node.
     * @param to_node Index of the ending node.
     * @return Reference to the flow edge between the given nodes.
     */
    FlowEdge& findEdge(std::size_t from_node, std::size_t to_node);

    /**
     * Helper function to show edges and nodes on new augmenting path.
     */
    void showFoundPath();

public:
    FordFulkersonGraphAlgorithm(FlowGraph&& graph);
    // TODO change this to iterator

    /**
     * Executes the next step of the Ford-Fulkerson graph algorithm.
     *
     * This method will continue to find augmenting paths and update the flow
     * until no more augmenting paths are found.
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