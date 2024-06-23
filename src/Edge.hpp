#ifndef Edge_hpp
#define Edge_hpp

#include "Drawable.hpp"

// Abstract base class representing an edge that can be drawn on a frame
class Edge : public Drawable {
public:
    virtual ~Edge() {};
};

#endif