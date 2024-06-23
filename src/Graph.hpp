#ifndef Graph_hpp
#define Graph_hpp

#include "Drawable.hpp"

// Abstract base class representing a graph that can be drawn on a frame
class Graph : public Drawable {
public:
    /**
     * Virtual destructor for Graph.
     * Allows proper cleanup when derived classes are deleted via a base class pointer.
     */
    virtual ~Graph() {};
};


#endif