
#include "FordFulkersonGraphAlgorithm.hpp"
#include "FlowGraph.hpp"

#include <limits>
#include <queue>
#include <stdexcept>

FordFulkersonGraphAlgorithm::FordFulkersonGraphAlgorithm(FlowGraph&& graph)
    :   m_graph(graph),
        m_start_node(graph.start_node),
        m_end_node(graph.end_node),
        m_max_flow(0),
        m_next_step_state(NextStepState::CleanGraph) {}

bool FordFulkersonGraphAlgorithm::BFS() {
    std::queue<std::size_t> queue;
    std::vector<bool> visited(m_graph.node_count, false);
    
    // Start BFS from the start node
    queue.push(m_start_node);
    visited[m_start_node] = true;
    m_parent.assign(m_graph.node_count, std::numeric_limits<std::size_t>::max());
    
    while (!queue.empty()) {
        std::size_t current_node = queue.front();
        queue.pop();
        
        for (std::size_t edge_index : m_graph.nodes[current_node].edges) {
            FlowEdge& edge = m_graph.edges[edge_index];
            std::size_t to_node = edge.to_node_index;
            
            // Check for unsaturated path
            if (!visited[to_node] && edge.current_flow < edge.capacity) {
                queue.push(to_node);
                visited[to_node] = true;
                m_parent[to_node] = current_node;
                
                // If end node is reached, an augmenting path is found
                if (to_node == m_end_node) {
                    return true;
                }
            }
        }
    }
    
    // No augmenting path found
    return false;
}

std::size_t FordFulkersonGraphAlgorithm::updateFlow() {
    // Find bottleneck capacity along the path from end node to start node
    std::size_t flow = std::numeric_limits<std::size_t>::max();
    for (std::size_t node = m_end_node; node != m_start_node; node = m_parent[node]) {
        std::size_t parent_node = m_parent[node];
        FlowEdge& edge = findEdge(parent_node, node);
        flow = std::min(flow, edge.capacity - edge.current_flow);
    }

    // Update the flow along the augmenting path
    for (std::size_t node = m_end_node; node != m_start_node; node = m_parent[node]) {
        std::size_t parent_node = m_parent[node];
        FlowEdge& edge = findEdge(parent_node, node);

        // Update forward edge
        edge.current_flow += flow;

        if (edge.current_flow == edge.capacity) {
            edge.state = FlowEdgeState::Saturated;
        }
    }
    return flow;
}


FlowEdge& FordFulkersonGraphAlgorithm::findEdge(std::size_t from_node, std::size_t to_node) {
    for (std::size_t edge_index : m_graph.nodes[from_node].edges) {
        FlowEdge& edge = m_graph.edges[edge_index];
        if (edge.to_node_index == to_node) {
            return edge;
        }
    }
    throw std::runtime_error("Edge not found between given nodes.");
}

void cleanGraph(FlowGraph& graph) {
    for (auto& node : graph.nodes) {
        node.state = FlowNodeState::Default;
    }
    for (auto& edge : graph.edges) {
        if (edge.current_flow == edge.capacity) {
            edge.state = FlowEdgeState::Saturated;
        }
        else {
            edge.state = FlowEdgeState::Default;
        }
    }
}

void FordFulkersonGraphAlgorithm::showFoundPath() {
    for (std::size_t node = m_end_node; node != m_start_node; node = m_parent[node]) {
        std::size_t parent_node = m_parent[node];
        m_graph.nodes[parent_node].state = FlowNodeState::OnUnsaturatedPath;
        FlowEdge& edge = findEdge(parent_node, node);
        edge.state = FlowEdgeState::OnUnsaturatedPath;
    }
}

bool FordFulkersonGraphAlgorithm::nextStep() {
    switch (m_next_step_state) {
        case NextStepState::CleanGraph:
            cleanGraph(m_graph);
            m_next_step_state = NextStepState::PathFind;
            return true;
        case NextStepState::PathFind:
            // Perform BFS to find an augmenting path
            if (!BFS()) {
                return false;
            }
            showFoundPath();
            m_next_step_state = NextStepState::UpdatePath;
            return true;
        case NextStepState::UpdatePath:
            // Update the flow along the augmenting path
            m_max_flow += updateFlow();
            m_next_step_state = NextStepState::CleanGraph;
            return true;
        default:
            // not gonna happen
            return false;
    }
}

const Graph& FordFulkersonGraphAlgorithm::getState() const {
    return m_graph;
}