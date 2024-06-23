#ifndef Node_hpp
#define Node_hpp

#include "Drawable.hpp"

// Abstract base class representing a node that can be drawn on a frame
class Node : public Drawable {
public:
    virtual ~Node() {};
};


#endif