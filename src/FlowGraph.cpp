
#include "FlowGraph.hpp"
#include "DrawHelpers.hpp"

#include <cstddef>

FlowNode::FlowNode(std::size_t pos_x, std::size_t pos_y, std::size_t size, 
        FlowNodeColorPalette palette, FlowNodeState state, FlowNodeType type)
    :   pos_x(pos_x), 
        pos_y(pos_y),
        m_size(size),
        m_color_palette(palette),
        state(state),
        type(type) {}

Color FlowNode::getNodeColor() const {
    if (type == FlowNodeType::Start) {
        return m_color_palette.start_node_color;
    }
    if (type == FlowNodeType::End) {
        return m_color_palette.end_node_color;
    }

    if (state == FlowNodeState::OnUnsaturatedPath) {
        return m_color_palette.on_unsaturated_path_color;
    }

    return m_color_palette.default_node_color;
}

void FlowNode::draw(Frame& frame) const {
    auto half_size = m_size / 2;
    auto top_left_x = pos_x - half_size;
    auto top_left_y = pos_y - half_size;
    auto bottom_right_x = pos_x + half_size;
    auto bottom_right_y = pos_y + half_size;

    auto node_color = getNodeColor();

    frame.drawRectangle(top_left_x, top_left_y, bottom_right_x, bottom_right_y, node_color);
}

FlowEdge::FlowEdge(std::size_t from, std::size_t to, std::size_t length, 
    std::size_t width, std::size_t border_width, std::size_t pos_x, std::size_t pos_y,
    bool is_horizontal, bool direction_from_top_left, std::size_t capacity, FlowEdgeState state, FlowEdgeColorPalette palette)
    :   from_node_index(from), 
        to_node_index(to), 
        m_length(length),
        m_width(width),
        m_border_width(border_width),
        m_pos_x(pos_x), 
        m_pos_y(pos_y),
        capacity(capacity),
        current_flow(0),
        m_is_horizontal(is_horizontal), 
        m_direction_from_top_left(direction_from_top_left),
        state(state),
        m_color_palette(palette) {}

Color FlowEdge::getEdgeColor() const {
    if (state == FlowEdgeState::OnUnsaturatedPath) {
        return m_color_palette.on_unsaturated_path_color;
    }
    if (state == FlowEdgeState::Saturated) {
        return m_color_palette.saturated_color;
    }

    return m_color_palette.default_edge_color;
}



void FlowEdge::draw(Frame& frame) const {
    auto horizontal_half_size = m_is_horizontal ? m_length / 2 : m_width / 2;
    auto vertical_half_size = m_is_horizontal ? m_width / 2 : m_length / 2;

    auto flow_size = (std::size_t)(m_width * ((double)current_flow / capacity));

    auto top_left_x = m_pos_x - horizontal_half_size;
    auto top_left_y = m_pos_y - vertical_half_size;
    auto bottom_right_x = m_pos_x + horizontal_half_size;
    auto bottom_right_y = m_pos_y + vertical_half_size;

    Color edge_color = getEdgeColor();
    Color border_offset = m_color_palette.border_color_offset;
    Color border_color = Color(
        std::max<std::size_t>(edge_color.R - border_offset.R, 0),
        std::max<std::size_t>(edge_color.G - border_offset.G, 0),
        std::max<std::size_t>(edge_color.B - border_offset.B, 0)
    );

    Color arrow_color = m_color_palette.arrow_color;

    auto square_size = m_border_width;
    auto arrow_width = m_border_width * 2;// (std::size_t)(m_length * 0.5);
    auto arrow_height =  m_border_width * 3;//(std::size_t)(m_width * 0.7);

    if (m_is_horizontal) {
        // Top border
        frame.drawRectangle(
            top_left_x, 
            top_left_y - m_border_width, 
            top_left_x + m_length, 
            top_left_y, 
            border_color);

        // Bottom border
        frame.drawRectangle(
            top_left_x, 
            bottom_right_y, 
            bottom_right_x, 
            bottom_right_y + m_border_width, 
            border_color);

        // Inner flow
        frame.drawRectangle(
            top_left_x, 
            top_left_y + m_width - flow_size, 
            bottom_right_x, 
            bottom_right_y, 
            edge_color);

        drawHorizontalArrow(
            m_pos_x, 
            m_pos_y, 
            arrow_width, 
            arrow_height, 
            square_size, 
            m_direction_from_top_left,
            frame,
            arrow_color);
    }
    else {
        // Right border
        frame.drawRectangle(
        bottom_right_x, 
        bottom_right_y - m_length, 
        bottom_right_x + m_border_width, 
        bottom_right_y, 
        border_color);

        // Left border
        frame.drawRectangle(
            top_left_x - m_border_width, 
            top_left_y, 
            top_left_x, 
            bottom_right_y, 
            border_color);

        // Inner flow
        frame.drawRectangle(
            top_left_x, 
            top_left_y, 
            bottom_right_x - m_width + flow_size, 
            bottom_right_y, 
            edge_color);

        arrow_width = m_border_width * 3;
        arrow_height = m_border_width * 2;

        drawVerticalArrow(
            m_pos_x, 
            m_pos_y, 
            arrow_width, 
            arrow_height, 
            square_size, 
            m_direction_from_top_left,
            frame,
            arrow_color);
    }
}

void FlowGraph::draw(Frame& frame) const {
    frame.drawRectangle(
        0, 
        0, 
        frame.width(),
        frame.height(), 
        m_background_color
    );

    for (std::size_t i = 0; i < edge_count; ++i) {
        FlowEdge edge = edges[i];
        edge.draw(frame);
    }

    for (std::size_t i = 0; i < node_count; ++i) {
        FlowNode node = nodes[i];
        node.draw(frame);
    }
}

std::pair<std::size_t, std::size_t> FlowGraph::getNodePosition(
    std::pair<std::size_t, std::size_t> indecies,
    const FlowGraphConfig& config) const {

    std::size_t margin = config.node_size + (config.node_size / 2); 
    std::size_t step = config.node_size + config.edge_length;

    auto first = margin + (indecies.first * step);
    auto second = margin + (indecies.second * step);
    return std::pair<std::size_t, std::size_t>(first, second);
}

static std::size_t getEdgeLength(const FlowNode& first, const FlowNode& second, bool is_horizontal, std::size_t node_size) {
    if (is_horizontal) {
        return std::max(first.pos_x, second.pos_x) - std::min(first.pos_x, second.pos_x) - node_size;
    } 
    else {
        return std::max(first.pos_y, second.pos_y) - std::min(first.pos_y, second.pos_y) - node_size;
    }
}

static std::pair<std::size_t, std::size_t> getEdgeCenterPos(const FlowNode& from, const FlowNode& to) {
    std::size_t pos_x = (from.pos_x + to.pos_x) / 2;
    std::size_t pos_y = (from.pos_y + to.pos_y) / 2;

    return std::pair(pos_x, pos_y);
}

static bool isEdgeDirectionFromTopLeft(const FlowNode& from, const FlowNode& to, bool is_horizontal) {
    if (is_horizontal && from.pos_x < to.pos_x) {
        return true;
    }
    if (!is_horizontal && from.pos_y < to.pos_y) {
        return true;
    }
    return false;
} 

void FlowGraph::populateNodesAndEdges(const FlowGraphConfig& config) {
    for (std::size_t i = 0; i < node_count; ++i) {
        auto position = getNodePosition(config.nodes[i], config);

        FlowNodeType type = (i == config.start_node) ? FlowNodeType::Start : FlowNodeType::Normal;
        type = (i == config.end_node) ? FlowNodeType::End : type;

        nodes[i] = FlowNode(
            position.first, 
            position.second,
            config.node_size,
            config.node_color_palette,
            FlowNodeState::Default,
            type
        );
    }

    for (std::size_t i = 0; i < edge_count; ++i) {
        auto from_index = config.edges[i].first.first;
        auto to_index = config.edges[i].first.second;
        auto is_horizontal = nodes[from_index].pos_y == nodes[to_index].pos_y;
        auto edge_length = getEdgeLength(nodes[from_index], nodes[to_index], is_horizontal, config.node_size);
        auto edge_center_pos = getEdgeCenterPos(nodes[from_index], nodes[to_index]);
        auto direction_from_top_left = isEdgeDirectionFromTopLeft(nodes[from_index], nodes[to_index], is_horizontal);

        auto edge_capacity = config.edges[i].second;
        std::size_t edge_border_width = config.edge_width * 0.1;
        auto edge_width = (std::size_t)(((double)edge_capacity / config.max_capacity) * config.edge_width);

        edges[i] = FlowEdge(
            from_index,
            to_index,
            edge_length,
            edge_width,
            edge_border_width,
            edge_center_pos.first,
            edge_center_pos.second,
            is_horizontal,
            direction_from_top_left,
            edge_capacity,
            FlowEdgeState::Default,
            config.edge_color_palette
        );

        nodes[from_index].edges.push_back(i);
    }
}

FlowGraph::FlowGraph(const FlowGraphConfig& config) 
    :   m_background_color(config.background_color),
        edge_count(config.edges.size()),
        node_count(config.nodes.size()),
        start_node(config.start_node),
        end_node(config.end_node),
        nodes(config.nodes.size()),
        edges(config.edges.size()) {

    populateNodesAndEdges(config);
}
