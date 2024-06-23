# Program Documentation

In here we describe the design, architecture and main classes and interfaces of the program.

## Overview

It is clear from our domain that we have to have some graph (with nodes and edges), graph algorithm and we have to be able to visualize it. Lets start with listing of most important interfaces used in our program.

### Interfaces

- [Drawable](../src/Drawable.hpp)
  - interface signalizing that class can be drawn on a [Frame](../src/Frame.hpp)
  - main (and only) method is ```void Draw(&Frame frame) const``` for drawing
- [Frame](../src/Frame.hpp)
  - abstraction for anything that has width and height and can be drawn on
  - main method is ```void drawRectangle(std::size_t left_upper_x, std::size_t left_upper_y, std::size_t right_lower_x, std::size_t right_lower_y, Color color)``` which draws a rectange on the frame
- [Graph](../src/Graph.hpp)
  - interface for drawable graph (it inherits from [Drawable](../src/Drawable.hpp))
  - every graph is responsible for drawing itself
- [Node](../src/Node.hpp)
  - interface for drawable node (it inherits from [Drawable](../src/Drawable.hpp))
- [Edge](../src/Edge.hpp)
  - interface for drawable edge (it inherits from [Drawable](../src/Drawable.hpp))
- [GraphAlgorithm](../src/GraphAlgorithm.hpp)
  - interface for stepable algorithm on a graph
  - has two main methods, ```bool nextStep()``` for proceeding with the next step and ```const Graph& getState() const``` for getting the information about changes in the graph
  - algorithm should in each step change the states of the graph nodes and edges so it can be rendered
- [Renderer](../src/Renderer.hpp)
  - interface for animation renderer facilitating drawing on each frame of the animation
  - main methods are ```Frame& beginDrawing()```, ```void endDrawing()``` and ```void finalize()```
  
### Classes

Now that we are familiar with the most important interfaces we can introduce important classes.

Most crutial class other than implementations of the afformentioned interfaces is the [GraphAlgorithmVisualizer](../src/GraphAlgorithmVisualizer.hpp). It takes implementations of [GraphAlgorithm](../src/GraphAlgorithm.hpp) and [Renderer](../src/Renderer.hpp) interfaces and with the main method ```void visualize()``` it steps through the whole algorithm run and draws each step with the [Renderer](../src/Renderer.hpp).

Rest of the important classes are basicaly implementations of the interfaces mentioned above.

- [PathGraph](../src/PathGraph.hpp), [PathNode](../src/PathGraph.hpp) and [PathEdge](../src/PathGraph.hpp)
  - implementation of the [Graph](../src/Graph.hpp), [Node](../src/Node.hpp) and [Edge](../src/Edge.hpp) interfaces for path-finding algorithms
- [FlowGraph](../src/FlowGraph.hpp), [FlowNode](../src/FlowGraph.hpp) and [FlowEdge](../src/FlowGraph.hpp)
  - implementation of the [Graph](../src/Graph.hpp), [Node](../src/Node.hpp) and [Edge](../src/Edge.hpp) interfaces for flow-finding algorithms
- [GIFRenderer](../src/GIFRenderer.hpp) and [GIFFrame](../src/GIFFrame.hpp)
  - implementations of the [Renderer](../src/Renderer.hpp) and [Frame](../src/Frame.hpp) interfaces for the animations in GIF file format
- [BFSGraphAlgorithm](../src/BFSGraphAlgorithm.hpp)
  - simple implementation of the Breadth-First-Search algorithm for finding the shortest path in a graph
- [FordFulkersonGraphAlgorithm](../src/FordFulkersonGraphAlgorithm.hpp)
  - simple implementation of the Ford-Fulkerson with BFS algorithm for finding the maximal flow in a flow network

## Extensibility

For anyone wishing to extend the range of algorithms provided the program is designed in such a way that it should be easily posible. [PathGraph](../src/PathGraph.hpp), [FlowGraph](../src/FlowGraph.hpp) and their respective nodes and edges should provide most of the common needs for path finding or flow finding algorithms. In that case the extention is a matter of implementing given algorithm with the correct [Graph](../src/Graph.hpp) implementaion.

If provided graphs are not suitable, user has to define one own and also needs to define config file for new graph and has to provide a method for loading such a config.

The extension of render options is also quite easily done. User just has to define its own [Renderer](../src/Renderer.hpp) and [Frame](../src/Frame.hpp) and the rest can stay the same.
