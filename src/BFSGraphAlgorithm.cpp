
#include "BFSGraphAlgorithm.hpp"
#include "PathGraph.hpp"

BFSGraphAlgorithm::BFSGraphAlgorithm(PathGraph&& graph) 
    : m_graph(std::move(graph)), m_unvisited_node_queue(), m_first_step(true), m_found_end(false) {}

bool BFSGraphAlgorithm::nextStep() {
    // Lets the renderer draw the initial state of graph
    if (m_first_step) {
        m_unvisited_node_queue.push(m_graph.start_node);
        m_first_step = false;
        return true;
    }

    // We are finished
    if (m_unvisited_node_queue.empty() || m_found_end) {
        return false;
    }

    auto node_index = m_unvisited_node_queue.front();
    m_unvisited_node_queue.pop();

    auto& node = m_graph.nodes[node_index];
    node.state = PathNodeState::Visited;
    if (!node.is_start_node) {
        m_graph.edges[node.entered_by].state = PathEdgeState::Used;
    }

    // We found the end node
    if (node.is_end_node) {
        if (node.is_start_node) {
            node.state = PathNodeState::OnShortestPath;
            m_found_end = true;
            return true;
        } 

        // Backtrack and mark the nodes and edges on the shortest path
        auto i = node.entered_by;
        while(m_graph.nodes[m_graph.edges[i].from_node_index].is_start_node != true) {
            m_graph.edges[i].state = PathEdgeState::OnShortestPath;
            auto& node = m_graph.nodes[m_graph.edges[i].from_node_index];
            node.state = PathNodeState::OnShortestPath;
            i = node.entered_by;
        }
        m_graph.edges[i].state = PathEdgeState::OnShortestPath;
        m_found_end = true;
        return true;
    }

    // Else we go throgh the neighbours and add them to queue
    for (auto edge_index : node.edges) {
        auto& edge = m_graph.edges[edge_index];

        auto neighbour_index = edge.to_node_index;
        auto& neighbour = m_graph.nodes[neighbour_index];

        if (neighbour.state == PathNodeState::NotFound) {
            edge.state = PathEdgeState::Peeked;

            m_unvisited_node_queue.push(neighbour_index);
            neighbour.state = PathNodeState::Found;
            neighbour.entered_by = edge_index;
        }
    }

    return true;
}

const Graph& BFSGraphAlgorithm::getState() const {
    return m_graph;
}
