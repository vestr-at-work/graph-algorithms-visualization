#ifndef Drawable_hpp
#define Drawable_hpp

#include "Frame.hpp"

// Abstract base class for objects that can be drawn on a frame
class Drawable {
public:
    /**
     * Draws the object on the given frame.
     *
     * @param frame The frame on which the object should be drawn.
     */
    virtual void draw(Frame& frame) const = 0;
    virtual ~Drawable() {};
};

#endif