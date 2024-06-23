#ifndef GraphAlgorithm_hpp
#define GraphAlgorithm_hpp

#include "Graph.hpp"

// Abstract base class for graph algorithms
class GraphAlgorithm {
public:
    // TODO change this to iterator

    /**
     * Executes the next step of the graph algorithm.
     *
     * @return True if there are more steps to be taken, false otherwise.
     */
    virtual bool nextStep() = 0;

    /**
     * Returns the current state of the graph.
     *
     * @return A constant reference to the current state of the graph.
     */
    virtual const Graph& getState() const = 0;

    /**
     * Virtual destructor for GraphAlgorithm.
     * Allows proper cleanup when derived classes are deleted via a base class pointer.
     */
    virtual ~GraphAlgorithm() {};
};

#endif