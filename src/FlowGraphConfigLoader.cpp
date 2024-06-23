
#include "FlowGraphConfigLoader.hpp"
#include "FlowGraph.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>

// Helper function to trim leading and trailing whitespace from a string
static std::string trim(const std::string& str) {
    std::size_t start = str.find_first_not_of(" \t\n\r");
    std::size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// Parsing functions for each section
void parseGridData(const std::string& line, FlowGraphConfig& config, std::size_t section_row) {
    std::istringstream iss(line);

    switch (section_row) {
        case 0:
            {
            std::string width_height;
            iss >> width_height;

            // Parse width and height
            std::size_t x_pos = width_height.find('x');
            if (x_pos == std::string::npos) {
                throw std::runtime_error("Invalid format for grid data: " + width_height);
            }

            config.grid_width = std::stoul(width_height.substr(0, x_pos));
            config.grid_height = std::stoul(width_height.substr(x_pos + 1));
            break;
            }
        case 1:
            // Parse start and end node indices
            if (!(iss >> config.start_node)) {
                throw std::runtime_error("Invalid format for grid data: missing start node index");
            }
            break;
        case 2:
            // Parse start and end node indices
            if (!(iss >> config.end_node)) {
                throw std::runtime_error("Invalid format for grid data: missing end node index");
            }
            break;
        default:
            throw std::runtime_error("Invalid grid data: unexpected line in the section");
    }
}

void parseNodes(const std::string& line, FlowGraphConfig& config) {
    std::istringstream iss(line);
    std::size_t node_x, node_y;

    // Parse node coordinates
    if (!(iss >> node_x >> node_y)) {
        throw std::runtime_error("Invalid format for node data: " + line);
    }

    // Add node to the configuration
    config.nodes.emplace_back(node_x, node_y);
}

std::size_t parseEdges(const std::string& line, FlowGraphConfig& config, std::size_t max_capacity) {
    std::istringstream iss(line);
    std::size_t from_node, to_node, capacity;

    // Parse edge data
    if (!(iss >> from_node >> to_node >> capacity)) {
        throw std::runtime_error("Invalid format for edge data: " + line);
    }

    if (from_node >= config.nodes.size() || to_node >= config.nodes.size()) {
        throw std::runtime_error("Invalid node index in edge data: " + line);
    }

    max_capacity = std::max<std::size_t>(max_capacity, capacity);

    // Add edge to the configuration
    config.edges.emplace_back(std::make_pair(from_node, to_node), capacity);

    return max_capacity;
}

void parseVisualization(const std::string& line, FlowGraphConfig& config, std::size_t section_row) {
    std::istringstream iss(line);

    switch(section_row) {
        case 0:
            if (!(iss >> config.node_size)) {
                throw std::runtime_error("Invalid format for visualization data node size: " + line);
            }
            if (!(config.node_size > 0)) {
                throw std::runtime_error("Invalid node size. Value must be bigger than 0");
            }
            break;
        case 1:
            if (!(iss >> config.edge_length)) {
                throw std::runtime_error("Invalid format for visualization data edge length: " + line);
            }
            if (!(config.edge_length > 0)) {
                throw std::runtime_error("Invalid edge length. Value must be bigger than 0");
            }
            break;
        case 2:
            if (!(iss >> config.edge_width)) {
                throw std::runtime_error("Invalid format for visualization data edge width: " + line);
            }
            if (!(config.edge_width > 0)) {
                throw std::runtime_error("Invalid edge width. Value must be bigger than 0");
            }
            break;
        case 3:
            if (!(iss >> config.frame_delay)) {
                throw std::runtime_error("Invalid format for visualization data frame delay: " + line);
            }
            if (!(config.frame_delay > 0)) {
                throw std::runtime_error("Invalid frame delay. Value must be bigger than 0");
            }
            break;
        case 4:
            // Parse background color
            int bg_r, bg_g, bg_b;
            if (!(iss >> bg_r >> bg_g >> bg_b)) {
                throw std::runtime_error("Invalid format for visualization data: " + line);
            }
            config.background_color = {static_cast<uint8_t>(bg_r), static_cast<uint8_t>(bg_g), static_cast<uint8_t>(bg_b)};
            break;
        default:
            throw std::runtime_error("Invalid visualization data: unexpected line in the section");
    }
}

void parseNodePalette(const std::string& line, FlowNodeColorPalette& palette, std::size_t section_row) {
    std::istringstream iss(line);
    int r, g, b;

    switch (section_row) {
        case 0:
            // Parse default node color
            if (!(iss >> r >> g >> b)) {
                throw std::runtime_error("Invalid format for node palette: default node color");
            }
            palette.default_node_color = {static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};
            break;
        case 1:
            // Parse on unsaturated path color
            if (!(iss >> r >> g >> b)) {
                throw std::runtime_error("Invalid format for node palette: on unsaturated path color");
            }
            palette.on_unsaturated_path_color = {static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};
            break;
        case 2:
            // Parse start node color
            if (!(iss >> r >> g >> b)) {
                throw std::runtime_error("Invalid format for node palette: start node color");
            }
            palette.start_node_color = {static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};
            break;
        case 3:
            // Parse end node color
            if (!(iss >> r >> g >> b)) {
                throw std::runtime_error("Invalid format for node palette: end node color");
            }
            palette.end_node_color = {static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};
            break;
        default:
            throw std::runtime_error("Invalid node palette data: unexpected line in the section");
    }
}

void parseEdgePalette(const std::string& line, FlowEdgeColorPalette& palette, std::size_t section_row) {
    std::istringstream iss(line);
    int r, g, b;

    switch (section_row) {
        case 0:
            // Parse default edge color
            if (!(iss >> r >> g >> b)) {
                throw std::runtime_error("Invalid format for edge palette: default edge color");
            }
            palette.default_edge_color = {static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};
            break;
        case 1:
            // Parse on unsaturated path color
            if (!(iss >> r >> g >> b)) {
                throw std::runtime_error("Invalid format for edge palette: on unsaturated path color");
            }
            palette.on_unsaturated_path_color = {static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};
            break;
        case 2:
            // Parse saturated color
            if (!(iss >> r >> g >> b)) {
                throw std::runtime_error("Invalid format for edge palette: saturated color");
            }
            palette.saturated_color = {static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};
            break;
        case 3:
            // Parse border color offset
            if (!(iss >> r >> g >> b)) {
                throw std::runtime_error("Invalid format for edge palette: border color offset");
            }
            palette.border_color_offset = {static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};
            break;
        case 4:
            // Parse arrow color
            if (!(iss >> r >> g >> b)) {
                throw std::runtime_error("Invalid format for edge palette: arrow color");
            }
            palette.arrow_color = {static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};
            break;
        default:
            throw std::runtime_error("Invalid edge palette data: unexpected line in the section");
    }
}

FlowGraphConfig loadFlowGraphConfig(std::istream& input) {
    FlowGraphConfig config;

    std::string line;
    std::string section; // Current section being parsed
    std::size_t section_row = 0; // Row index in the current section
    std::size_t max_capacity = 0;

    while (std::getline(input, line)) {
        line = trim(line);

        // Ignore empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Check for section headers
        if (line[0] == '[' && line.back() == ']') {
            section = line;
            section_row = 0;
            continue;
        }

        // Parse data based on the current section
        if (section == "[GRID DATA]") {
            parseGridData(line, config, section_row);
            section_row++;
        } else if (section == "[NODES]") {
            parseNodes(line, config);
        } else if (section == "[EDGES]") {
            max_capacity = parseEdges(line, config, max_capacity);
        } else if (section == "[VISUALIZATION]") {
            parseVisualization(line, config, section_row);
            section_row++;
        } else if (section == "[NODE PALETTE]") {
            parseNodePalette(line, config.node_color_palette, section_row);
            section_row++;
        } else if (section == "[EDGE PALETTE]") {
            parseEdgePalette(line, config.edge_color_palette, section_row);
            section_row++;
        } else {
            throw std::runtime_error("Unknown section: " + section);
        }
    }

    config.max_capacity = max_capacity;
    return config;
};