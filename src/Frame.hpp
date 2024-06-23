#ifndef Frame_hpp
#define Frame_hpp

#include "Color.hpp"
#include <cstddef>

// Abstract base class representing a frame where drawing operations can be performed
class Frame {
public:
    /**
     * Draws a rectangle on the frame.
     *
     * @param left_upper_x The x-coordinate of the top-left corner of the rectangle.
     * @param left_upper_y The y-coordinate of the top-left corner of the rectangle.
     * @param right_lower_x The x-coordinate of the bottom-right corner of the rectangle.
     * @param right_lower_y The y-coordinate of the bottom-right corner of the rectangle.
     * @param color The color of the rectangle.
     */
    virtual void drawRectangle(std::size_t left_upper_x, std::size_t left_upper_y,
        std::size_t right_lower_x, std::size_t right_lower_y, Color color) = 0;

    /**
     * Returns the width of the frame.
     *
     * @return The width of the frame.
     */
    virtual std::size_t width() const = 0;

    /**
     * Returns the height of the frame.
     *
     * @return The height of the frame.
     */
    virtual std::size_t height() const = 0;

    virtual ~Frame() {};
};

#endif