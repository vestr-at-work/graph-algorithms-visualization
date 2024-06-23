#ifndef Renderer_hpp
#define Renderer_hpp

#include "Frame.hpp"

// Abstract base class representing a renderer for drawing on frames
class Renderer {
public:
    /**
     * Begins drawing a new frame.
     *
     * This method is expected to be called before any drawing operations on a frame.
     * The returned frame will be used to perform drawing operations.
     *
     * @return A reference to the frame to be drawn.
     */
    virtual Frame& beginDrawing() = 0;

    /**
     * Ends drawing the current frame.
     *
     * This method is expected to be called after drawing operations on a frame are completed.
     * It typically finalizes the drawing and prepares the frame for further processing or output.
     */
    virtual void endDrawing() = 0;

    /**
     * Finalizes the rendering process.
     *
     * This method is used to finalize the rendering process and may contain any necessary
     * cleanup or finalization steps. By default, the method does nothing but can be overridden
     * in derived classes to perform specific finalization tasks.
     */
    virtual void finalize() {};

    virtual ~Renderer() {};
};


#endif