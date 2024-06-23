
#include "BFSGraphAlgorithm.hpp"
#include "FlowGraph.hpp"
#include "FlowGraphConfigLoader.hpp"
#include "FordFulkersonGraphAlgorithm.hpp"
#include "GIFRenderer.hpp"
#include "GraphAlgorithmVisualizer.hpp"
#include "PathGraph.hpp"
#include "PathGraphConfigLoader.hpp"
#include <cstddef>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>


void printHelp(const std::string& command) {
    std::cout << "Usage: " << command << " ALGORITHM GRAPH_CONFIG_FILE OUTPUT_FILE" << std::endl;
    std::cout << "Visualize a run of given ALGORITHM on a (grid) graph defined in a GRAPH_CONFIG_FILE and save it to OUTPUT_FILE." << std::endl;
    std::cout << std::endl;
    std::cout << "Only supported ALGORITHMs are:" << std::endl;
    std::cout << "  BFS (Breadth-First-Search) shortest path finding algorithm" << std::endl;
    std::cout << "  FF-BFS (Ford-Fulkerson with BFS) maximal flow finding algorithm" << std::endl;
    std::cout << std::endl;
    std::cout << "Only supported output format is .gif" << std::endl;
}

std::size_t getFrameDimension(std::size_t node_size, std::size_t edge_length, std::size_t dimension) {
    return node_size * (dimension + 2) + edge_length * (dimension - 1);
}

int main(int argc, const char *argv[]) {
    if (argc == 2 && std::string(argv[1]) == "--help") {
        printHelp(argv[0]);
        return 0;
    }

    if (argc < 3) {
		std::cout << "Error: Missing mandatory parameters" << std::endl;
        std::cout << std::endl;
        printHelp(argv[0]);
		return 1;
	}
    std::string alg_name = argv[1];
	std::ifstream input_stream(argv[2]);
    std::string output_file_name = argv[3];

	if (!input_stream.good()) {
		std::cout << "Error: Can not open file: \"" << argv[2] << "\"" << std::endl;
		return 1;
	}

    try {
        if (alg_name == "FF-BFS") {
            auto config = loadFlowGraphConfig(input_stream);
            auto frame_width = getFrameDimension(config.node_size, config.edge_length, config.grid_width);
            auto frame_height = getFrameDimension(config.node_size, config.edge_length, config.grid_height);
            auto delay = config.frame_delay;
            FlowGraph graph(config);

            auto renderer_ptr = std::make_unique<GIFRenderer>(output_file_name, delay, frame_width, frame_height);
            auto algo_ptr = std::make_unique<FordFulkersonGraphAlgorithm>(std::move(graph));

            GraphAlgorithmVisualizer visualizer(std::move(algo_ptr), std::move(renderer_ptr));
            visualizer.visualize();
            return 0;
        }
        if (alg_name == "BFS") {
            auto config = loadPathGraphConfig(input_stream);
            auto frame_width = getFrameDimension(config.node_size, config.edge_length, config.grid_width);
            auto frame_height = getFrameDimension(config.node_size, config.edge_length, config.grid_height);
            auto delay = config.frame_delay;
            PathGraph graph(config);

            auto renderer_ptr = std::make_unique<GIFRenderer>(output_file_name, delay, frame_width, frame_height);
            auto algo_ptr = std::make_unique<BFSGraphAlgorithm>(std::move(graph));

            GraphAlgorithmVisualizer visualizer(std::move(algo_ptr), std::move(renderer_ptr));
            visualizer.visualize();
            return 0;
        }

        std::cout << "Error: Unknown algorithm \"" << argv[1] << "\"" << std::endl;
		return 1;
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cout << "Unknown error occured." << std::endl;
        return 1;
    }
}
