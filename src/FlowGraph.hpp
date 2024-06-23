#ifndef FlowGraph_hpp
#define FlowGraph_hpp

#include "Edge.hpp"
#include "Graph.hpp"
#include "Node.hpp"
#include <cstddef>
#include <vector>

// Enumeration representing the state of a flow node
enum class FlowNodeState { Default, OnUnsaturatedPath };

// Enumeration representing the type of a flow node
enum class FlowNodeType { Normal, Start, End };

// Enumeration representing the state of a flow edge
enum class FlowEdgeState { Default, Saturated, OnUnsaturatedPath };

class FlowEdge;

struct FlowNodeColorPalette {
    Color start_node_color,
    end_node_color,
    default_node_color,
    on_unsaturated_path_color;
};

struct FlowEdgeColorPalette {
    Color default_edge_color,
    saturated_color,
    on_unsaturated_path_color,
    border_color_offset,
    arrow_color;
};

// Class representing a node in the flow graph.
class FlowNode : public Node {
    std::size_t m_size;
    FlowNodeColorPalette m_color_palette;

    Color getNodeColor() const;
public: 
    std::size_t pos_x, pos_y;
    FlowNodeState state;
    FlowNodeType type;
    std::vector<std::size_t> edges; // List of edges connected to the node

    // Default constructor for FlowNode
    FlowNode() {};

    /**
     * Constructor for FlowNode.
     *
     * @param pos_x x-coordinate of the node center position.
     * @param pos_y y-coordinate of the node center position.
     * @param size Size of the node.
     * @param palette Color palette for the node.
     * @param state State of the node.
     * @param type Type of the node.
     */
    FlowNode(std::size_t pos_x, std::size_t pos_y, std::size_t size,
        FlowNodeColorPalette palette, FlowNodeState state, FlowNodeType type);
    
    /**
     * Draws the flow node on the given frame.
     *
     * @param frame The frame on which the node should be drawn.
     */
    virtual void draw(Frame& frame) const override;
};

// Class representing an edge in the flow graph.
class FlowEdge : public Edge {
    FlowEdgeColorPalette m_color_palette;
    std::size_t m_length, m_width, m_border_width; 
    std::size_t m_pos_x, m_pos_y; // Position of the center of the edge (x, y)
    bool m_is_horizontal;
    bool m_direction_from_top_left; // True if direction of the edge is from top down or left to right

    Color getEdgeColor() const;
public:
    std::size_t capacity, current_flow;
    FlowEdgeState state;
    std::size_t from_node_index;
    std::size_t to_node_index;

    // Default constructor for FlowEdge
    FlowEdge() {};

    /**
     * Constructor for FlowEdge.
     *
     * @param from Index of the starting node.
     * @param to Index of the ending node.
     * @param length Length of the edge.
     * @param width Width of the edge.
     * @param border_width Width of the border of the edge.
     * @param pos_x x-coordinate of the edge center position.
     * @param pos_y y-coordinate of the edge center position.
     * @param is_horizontal True if the edge is horizontal, false otherwise.
     * @param direction_from_top_left True if direction of the edge is from top down or left to right, false otherwise.
     * @param capacity Capacity of the edge.
     * @param state State of the edge.
     * @param palette Color palette for the edge.
     */
    FlowEdge(std::size_t from, std::size_t to, std::size_t length,
        std::size_t width, std::size_t border_width, std::size_t pos_x, std::size_t pos_y,
        bool is_horizontal, bool direction_from_top_left, std::size_t capacity, FlowEdgeState state, 
        FlowEdgeColorPalette palette);

    /**
     * Draws the flow edge on the given frame.
     *
     * @param frame The frame on which the edge should be drawn.
     */
    virtual void draw(Frame& frame) const override;
};

// Structure representing the configuration for a flow graph
struct FlowGraphConfig {
    std::size_t grid_width; // Width of the grid
    std::size_t grid_height; // Height of the grid
    std::vector<std::pair<std::size_t, std::size_t>> nodes; // List of node coordinates in the grid graph
    std::vector<std::pair<std::pair<std::size_t, std::size_t>, std::size_t>> edges; 
    // List of pair of nodes to be connected by an edge in the graph and the capacity. 
    // In the inner pair the first is 'from' and the second is 'to'.
    std::size_t start_node; // Index of the start node
    std::size_t end_node; // Index of the end node
    FlowNodeColorPalette node_color_palette; // Color palette for flow nodes
    FlowEdgeColorPalette edge_color_palette; // Color palette for flow edges
    Color background_color; // Background color of the graph
    std::size_t node_size; // Size of the flow nodes
    std::size_t edge_length; // Length of the flow edges
    std::size_t edge_width; // Width of the flow edges
    std::size_t max_capacity; // Highest capacity of the edges
    std::size_t frame_delay; // Frame delay in tens of miliseconds
};

// Class representing a flow graph, derived from the Graph base class
class FlowGraph : public Graph {
    Color m_background_color;

     /**
     * Populates the nodes and edges of the graph based on the configuration.
     *
     * @param config The configuration for the flow graph.
     */
    void populateNodesAndEdges(const FlowGraphConfig& config);

    /**
     * Returns the position of a node based on its indices in the configuration.
     *
     * @param indices The indices of the node.
     * @param config The configuration for the flow graph.
     * @return A pair containing the x and y coordinates of the node position in the grid graph.
     */
    std::pair<std::size_t, std::size_t> getNodePosition(
        std::pair<std::size_t, std::size_t> indices,
        const FlowGraphConfig& config) const;

public:
    std::size_t edge_count;
    std::size_t node_count;
    std::size_t start_node; // Index of the start node
    std::size_t end_node; // Index of the end node
    std::vector<FlowEdge> edges;
    std::vector<FlowNode> nodes;

    FlowGraph(const FlowGraphConfig& config);

    /**
     * Draws the flow graph on the given frame.
     *
     * @param frame The frame on which the flow graph should be drawn.
     */
    virtual void draw(Frame& frame) const override;
};

#endif