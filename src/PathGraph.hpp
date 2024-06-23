#ifndef PathGraph_hpp
#define PathGraph_hpp

#include "Edge.hpp"
#include "Graph.hpp"
#include "Node.hpp"
#include <cstddef>
#include <vector>

// Enumeration representing the state of a path node
enum class PathNodeState { NotFound, Found, Visited, OnShortestPath };

// Enumeration representing the state of a path edge
enum class PathEdgeState { Default, Peeked, Used, OnShortestPath };


class PathNode;

// Structure representing the color palette for a path node
struct PathNodeColorPalette {
    Color start_node_color,
    end_node_color, 
    found_unvisited_node_color,
    unfound_node_color,
    visited_node_color,
    on_shortest_path_color;
};

// Structure representing the color palette for a path edge
struct PathEdgeColorPalette {
    Color default_edge_color,
    peeked_edge_color,
    used_edge_color,
    on_shortest_path_color,
    arrow_color;
};

// Class representing a path edge in a graph, derived from the Edge base class
class PathEdge : public Edge {
    PathEdgeColorPalette m_color_palette;
    std::size_t m_length, m_width; 
    std::size_t m_pos_x, m_pos_y; // Position of the center of the edge (x, y)
    bool m_is_horizontal;
    bool m_direction_from_top_left; // True if direction of the edge is from top down or left to right

public:
    PathEdgeState state;
    std::size_t from_node_index;
    std::size_t to_node_index;

    // Default constructor for PathEdge
    PathEdge();

    /**
     * Constructor for PathEdge.
     *
     * @param from Index of the starting node.
     * @param to Index of the ending node.
     * @param length Length of the edge.
     * @param width Width of the edge.
     * @param pos_x x-coordinate of the edge center position.
     * @param pos_y y-coordinate of the edge center position.
     * @param is_horizontal True if the edge is horizontal, false otherwise.
     * @param direction_from_top_left True if direction of the edge is from top down or left to right, false otherwise.
     * @param state State of the edge.
     * @param palette Color palette for the edge.
     */
    PathEdge(std::size_t from, std::size_t to, std::size_t length,
        std::size_t width, std::size_t pos_x, std::size_t pos_y,
        bool is_horizontal, bool direction_from_top_left, PathEdgeState state, 
        PathEdgeColorPalette palette);

    /**
     * Draws the path edge on the given frame.
     *
     * @param frame The frame on which the path edge should be drawn.
     */
    virtual void draw(Frame& frame) const override;
};

// Class representing a path node in a graph, derived from the Node base class
class PathNode : public Node {
    std::size_t m_size;
    PathNodeColorPalette m_color_palette;

    /**
     * Returns the color of the node based on its state.
     *
     * @return The color of the node.
     */
    Color getNodeColor() const;

public:
    std::size_t pos_x, pos_y; // Position of the center of the node (x, y)
    PathNodeState state;
    bool is_start_node, is_end_node; 
    std::size_t entered_by; // Index of the edge the node was entered by
    std::vector<std::size_t> edges; // List of edges connected to the node

    // Default constructor for PathNode
    PathNode() {};

    /**
     * Constructor for PathNode.
     *
     * @param pos_x x-coordinate of the node center position.
     * @param pos_y y-coordinate of the node center position.
     * @param size Size of the node.
     * @param palette Color palette for the node.
     * @param state State of the node.
     * @param type Type of the node.
     */
    PathNode(std::size_t pos_x, std::size_t pos_y, std::size_t size,
        PathNodeColorPalette palette, PathNodeState state, bool is_start, bool is_end);

    /**
     * Draws the path node on the given frame.
     *
     * @param frame The frame on which the path node should be drawn.
     */
    virtual void draw(Frame& frame) const override;
};

// Structure representing the configuration for a path graph
struct PathGraphConfig {
    std::size_t grid_width; // Width of the grid
    std::size_t grid_height; // Height of the grid
    std::vector<std::pair<std::size_t, std::size_t>> nodes; // List of node coordinates in the grid graph
    std::vector<std::pair<std::size_t, std::size_t>> edges; // List of pair of nodes to be connected by an edge in the graph. 
                                                            // First is 'from' second is 'to'
    std::size_t start_node; // Index of the start node
    std::size_t end_node; // Index of the end node
    PathNodeColorPalette node_color_palette; // Color palette for path nodes
    PathEdgeColorPalette edge_color_palette; // Color palette for path edges
    Color background_color; // Background color of the graph
    std::size_t node_size; // Size of the path nodes
    std::size_t edge_length; // Length of the path edges
    std::size_t edge_width; // Width of the path edges
    std::size_t frame_delay; // Frame delay in tens of miliseconds
};

// Class representing a path graph, derived from the Graph base class
class PathGraph : public Graph {
    Color m_background_color; // Background color of the graph

    /**
     * Populates the nodes and edges of the graph based on the configuration.
     *
     * @param config The configuration for the path graph.
     */
    void populateNodesAndEdges(const PathGraphConfig& config);

    /**
     * Returns the position of a node based on its indices in the configuration.
     *
     * @param indices The indices of the node.
     * @param config The configuration for the path graph.
     * @return A pair containing the x and y coordinates of the node position in the grid graph.
     */
    std::pair<std::size_t, std::size_t> getNodePosition(
        std::pair<std::size_t, std::size_t> indices,
        const PathGraphConfig& config) const;

public:
    std::size_t edge_count;
    std::size_t node_count;
    std::size_t start_node; // Index of the start node
    std::size_t end_node; // Index of the end node
    std::vector<PathEdge> edges;
    std::vector<PathNode> nodes;

    /**
     * Constructor for PathGraph.
     *
     * @param config The configuration for the path graph.
     */
    PathGraph(const PathGraphConfig& config);

    /**
     * Draws the path graph on the given frame.
     *
     * @param frame The frame on which the path graph should be drawn.
     */
    virtual void draw(Frame& frame) const override;
};


#endif