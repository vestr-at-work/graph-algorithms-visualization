#ifndef GIFFrame_hpp
#define GIFFrame_hpp

#include "Color.hpp"
#include "Frame.hpp"
#include "gif.hpp"
#include <cstddef>

// Class representing a frame for GIF images, derived from the Frame base class
class GIFFrame : public Frame {
    std::size_t m_width;
    std::size_t m_height; 
    gif::Frame m_frame; // Internal representation of the frame

public:
    /**
     * Constructor for GIFFrame.
     *
     * @param width The width of the GIF frame.
     * @param height The height of the GIF frame.
     */
    GIFFrame(std::size_t width, std::size_t height);

    /**
     * Draws a rectangle on the GIF frame. Overriden from Frame class.
     *
     * @param left_upper_x The x-coordinate of the top-left corner of the rectangle.
     * @param left_upper_y The y-coordinate of the top-left corner of the rectangle.
     * @param right_lower_x The x-coordinate of the bottom-right corner of the rectangle.
     * @param right_lower_y The y-coordinate of the bottom-right corner of the rectangle.
     * @param color The color of the rectangle.
     */
    virtual void drawRectangle(std::size_t left_upper_x, std::size_t left_upper_y,
        std::size_t right_lower_x, std::size_t right_lower_y, Color color) override;

    /**
     * Returns the width of the GIF frame.
     *
     * @return The width of the GIF frame.
     */
    virtual std::size_t width() const override;

    /**
     * Returns the height of the GIF frame.
     *
     * @return The height of the GIF frame.
     */
    virtual std::size_t height() const override;

    /**
     * Returns the internal representation of the frame.
     *
     * @return A constant reference to the internal representation of the frame.
     */
    const gif::Frame& getFrame() const;
};

#endif