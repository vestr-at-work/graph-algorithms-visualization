#ifndef GraphAlgorithmVisualizer_hpp
#define GraphAlgorithmVisualizer_hpp

#include "GraphAlgorithm.hpp"
#include "Renderer.hpp"
#include <memory>

// Class for visualizing the whole run of graph algorithm
class GraphAlgorithmVisualizer {
    std::unique_ptr<GraphAlgorithm> m_algorithm;
    std::unique_ptr<Renderer> m_renderer;

public:
    /**
     * Constructor for GraphAlgorithmVisualizer.
     *
     * @param algorithm A unique pointer to the graph algorithm to be visualized.
     * @param renderer A unique pointer to the renderer used for visualization.
     */
    GraphAlgorithmVisualizer(std::unique_ptr<GraphAlgorithm> algorithm, std::unique_ptr<Renderer> renderer)
        : m_algorithm(std::move(algorithm)), m_renderer(std::move(renderer)) {}

    /**
     * Visualizes the entire run of the graph algorithm.
     * 
     * The method executes the algorithm step by step and visualizes each step using the renderer.
     */
    void visualize() {
        while (m_algorithm->nextStep()) {
            Frame& nextFrame = m_renderer->beginDrawing();

            // Get the state of the graph from the algorithm
            auto& nextGraph = m_algorithm->getState();
            nextGraph.draw(nextFrame);

            m_renderer->endDrawing();
        }
        m_renderer->finalize();
    }
};

#endif