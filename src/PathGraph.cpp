#include "PathGraph.hpp"
#include "DrawHelpers.hpp"
#include "Frame.hpp"
#include <cstddef>
#include <cstdlib>

PathNode::PathNode(std::size_t pos_x, std::size_t pos_y, std::size_t size, 
        PathNodeColorPalette palette, PathNodeState state, bool is_start_node, bool is_end_node)
    :   pos_x(pos_x), 
        pos_y(pos_y),
        m_size(size),
        m_color_palette(palette),
        state(state),
        is_start_node(is_start_node),
        is_end_node(is_end_node) {}

Color PathNode::getNodeColor() const {
    if (state == PathNodeState::OnShortestPath) {
        return m_color_palette.on_shortest_path_color;
    }

    if (is_start_node) {
        return m_color_palette.start_node_color;
    }
    if (is_end_node) {
        return m_color_palette.end_node_color;
    }

    if (state == PathNodeState::Visited) {
        return m_color_palette.visited_node_color;
    }
    if (state == PathNodeState::Found) {
        return m_color_palette.found_unvisited_node_color;
    }

    return m_color_palette.unfound_node_color;
}

PathEdge::PathEdge(std::size_t from, std::size_t to, std::size_t length, 
    std::size_t width, std::size_t pos_x, std::size_t pos_y,
    bool is_horizontal, bool direction_from_top_left, PathEdgeState state, 
    PathEdgeColorPalette palette)
    :   from_node_index{from}, 
        to_node_index{to}, 
        m_length{length},
        m_width{width}, 
        m_pos_x{pos_x}, 
        m_pos_y{pos_y},
        m_is_horizontal{is_horizontal},
        m_direction_from_top_left(direction_from_top_left),
        state{state},
        m_color_palette{palette} {}

PathEdge::PathEdge() {} 

void PathNode::draw(Frame& frame) const {
    auto half_size = m_size / 2;
    auto top_left_x = pos_x - half_size;
    auto top_left_y = pos_y - half_size;
    auto bottom_right_x = pos_x + half_size;
    auto bottom_right_y = pos_y + half_size;

    auto node_color = getNodeColor();

    frame.drawRectangle(top_left_x, top_left_y, bottom_right_x, bottom_right_y, node_color);
}

void PathEdge::draw(Frame& frame) const {
    auto horizontal_half_size = m_is_horizontal ? m_length / 2 : m_width / 2;
    auto vertical_half_size = m_is_horizontal ? m_width / 2 : m_length / 2;

    auto top_left_x = m_pos_x - horizontal_half_size;
    auto top_left_y = m_pos_y - vertical_half_size;
    auto bottom_right_x = m_pos_x + horizontal_half_size;
    auto bottom_right_y = m_pos_y + vertical_half_size;

    Color edge_color = m_color_palette.default_edge_color;
    if (state == PathEdgeState::Used) {
        edge_color = m_color_palette.used_edge_color;
    }
    if (state == PathEdgeState::Peeked) {
        edge_color = m_color_palette.peeked_edge_color;
    }
    if (state == PathEdgeState::OnShortestPath) {
        edge_color = m_color_palette.on_shortest_path_color;
    }

    // draw edge
    frame.drawRectangle(
        top_left_x, 
        top_left_y, 
        bottom_right_x, 
        bottom_right_y, 
        edge_color);

    auto square_size = (std::size_t)(m_width * 0.2);
    auto arrow_width = square_size * 2;
    auto arrow_height = square_size * 3;

    // draw arrow
    if (m_is_horizontal) {
        drawHorizontalArrow(
            m_pos_x, 
            m_pos_y, 
            arrow_width, 
            arrow_height, 
            square_size, 
            m_direction_from_top_left,
            frame,
            m_color_palette.arrow_color);
    }
    else {
        arrow_width = square_size * 3;
        arrow_height = square_size * 2;

        drawVerticalArrow(
            m_pos_x, 
            m_pos_y, 
            arrow_width, 
            arrow_height, 
            square_size, 
            m_direction_from_top_left,
            frame,
            m_color_palette.arrow_color);
    }
}

PathGraph::PathGraph(const PathGraphConfig& config) 
    :   m_background_color(config.background_color),
        edge_count(config.edges.size()),
        node_count(config.nodes.size()),
        start_node(config.start_node),
        end_node(config.end_node),
        nodes(config.nodes.size()),
        edges(config.edges.size()) {

    populateNodesAndEdges(config);
} 

void PathGraph::draw(Frame& frame) const {
    frame.drawRectangle(
        0, 
        0, 
        frame.width(),
        frame.height(), 
        m_background_color
    );

    for (std::size_t i = 0; i < edge_count; ++i) {
        PathEdge edge = edges[i];
        edge.draw(frame);
    }

    for (std::size_t i = 0; i < node_count; ++i) {
        PathNode node = nodes[i];
        node.draw(frame);
    }
}

std::pair<std::size_t, std::size_t> PathGraph::getNodePosition(
    std::pair<std::size_t, std::size_t> indecies,
    const PathGraphConfig& config) const {

    std::size_t margin = config.node_size + (config.node_size / 2); 
    std::size_t step = config.node_size + config.edge_length;

    auto first = margin + (indecies.first * step);
    auto second = margin + (indecies.second * step);
    return std::pair<std::size_t, std::size_t>(first, second);
}

// TODO make PathNode and FlowNode derived from parent node with position

static std::size_t getEdgeLength(const PathNode& first, const PathNode& second, bool is_horizontal, std::size_t node_size) {
    if (is_horizontal) {
        return std::max(first.pos_x, second.pos_x) - std::min(first.pos_x, second.pos_x) - node_size;
    } 
    else {
        return std::max(first.pos_y, second.pos_y) - std::min(first.pos_y, second.pos_y) - node_size;
    }
}

static std::pair<std::size_t, std::size_t> getEdgeCenterPos(const PathNode& from, const PathNode& to) {
    std::size_t pos_x = (from.pos_x + to.pos_x) / 2;
    std::size_t pos_y = (from.pos_y + to.pos_y) / 2;

    return std::pair(pos_x, pos_y);
}

static bool isEdgeDirectionFromTopLeft(const PathNode& from, const PathNode& to, bool is_horizontal) {
    if (is_horizontal && from.pos_x < to.pos_x) {
        return true;
    }
    if (!is_horizontal && from.pos_y < to.pos_y) {
        return true;
    }
    return false;
}

void PathGraph::populateNodesAndEdges(const PathGraphConfig& config) {
    for (std::size_t i = 0; i < node_count; ++i) {
        auto position = getNodePosition(config.nodes[i], config);

        nodes[i] = PathNode(
            position.first, 
            position.second,
            config.node_size,
            config.node_color_palette,
            PathNodeState::NotFound,
            i == config.start_node,
            i == config.end_node
        );
    }

    for (std::size_t i = 0; i < edge_count; ++i) {
        auto from_index = config.edges[i].first;
        auto to_index = config.edges[i].second;
        auto is_horizontal = nodes[from_index].pos_y == nodes[to_index].pos_y;
        auto edge_length = getEdgeLength(nodes[from_index], nodes[to_index], is_horizontal, config.node_size);
        auto edge_center_pos = getEdgeCenterPos(nodes[from_index], nodes[to_index]);
        auto direction_from_top_left = isEdgeDirectionFromTopLeft(nodes[from_index], nodes[to_index], is_horizontal);
        
        edges[i] = PathEdge(
            from_index,
            to_index,
            edge_length,
            config.edge_width,
            edge_center_pos.first,
            edge_center_pos.second,
            is_horizontal,
            direction_from_top_left,
            PathEdgeState::Default,
            config.edge_color_palette
        );

        nodes[from_index].edges.push_back(i);
    }
}