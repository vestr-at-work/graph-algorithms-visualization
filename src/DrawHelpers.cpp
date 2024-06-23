
#include "DrawHelpers.hpp"

/**
 * @brief Draws a vertical arrow with squares at each end.
 * 
 * @param pos_x The x-coordinate of the arrow's center.
 * @param pos_y The y-coordinate of the arrow's center.
 * @param arrow_width The width of the arrow.
 * @param arrow_height The height of the arrow.
 * @param square_size The size of the squares at the ends of the arrow.
 * @param direction_from_top If true, the arrow points downward; if false, the arrow points upward.
 * @param frame The frame on which to draw the arrow.
 * @param color The color of the arrow squares.
 */
void drawVerticalArrow(std::size_t pos_x, std::size_t pos_y, std::size_t arrow_width, std::size_t arrow_height, 
            std::size_t square_size, bool direction_from_top, Frame& frame, const Color& color) {
    
    auto left_square_x = pos_x - arrow_width / 2; // top left point of the square
    auto left_square_y = pos_y - arrow_height / 2; // top left point of the square

    auto right_square_x = (pos_x + arrow_width / 2) - square_size + (arrow_width % 2); // top left point of the square
    auto right_square_y = pos_y - arrow_height / 2; // top left point of the square

    auto point_square_x = pos_x - square_size / 2; // top left point of the square
    auto point_square_y = (pos_y + arrow_height / 2) - square_size + (arrow_height % 2); // top left point of the square

    if (!direction_from_top) {
        left_square_y += arrow_height - square_size;
        right_square_y += arrow_height - square_size;
        point_square_y -= arrow_height - square_size;
    }

    // draw left square
    frame.drawRectangle(
        left_square_x, 
        left_square_y,
        left_square_x + square_size,
        left_square_y + square_size, 
        color);

    // draw right square
    frame.drawRectangle(
        right_square_x, 
        right_square_y,
        right_square_x + square_size,
        right_square_y + square_size, 
        color);

    // draw point square
    frame.drawRectangle(
        point_square_x, 
        point_square_y,
        point_square_x + square_size,
        point_square_y + square_size, 
        color);
}

/**
 * @brief Draws a horizontal arrow with squares at each end.
 * 
 * @param pos_x The x-coordinate of the arrow's center.
 * @param pos_y The y-coordinate of the arrow's center.
 * @param arrow_width The width of the arrow.
 * @param arrow_height The height of the arrow.
 * @param square_size The size of the squares at the ends of the arrow.
 * @param direction_from_left If true, the arrow points to the right; if false, the arrow points to the left.
 * @param frame The frame on which to draw the arrow.
 * @param color The color of the arrow squares.
 */
void drawHorizontalArrow(std::size_t pos_x, std::size_t pos_y, std::size_t arrow_width, std::size_t arrow_height, 
            std::size_t square_size, bool direction_from_left, Frame& frame, const Color& color) {
    
    auto upper_square_x = pos_x - arrow_width / 2; // top left point of the square
    auto upper_square_y = pos_y - arrow_height / 2; // top left point of the square

    auto lower_square_x = pos_x - arrow_width / 2; // top left point of the square
    auto lower_square_y = (pos_y + arrow_height / 2) - square_size + (arrow_height % 2); // top left point of the square

    auto point_square_x = (pos_x + arrow_width / 2) - square_size + (arrow_width % 2); // top left point of the square
    auto point_square_y = pos_y - square_size / 2; // top left point of the square

    if (!direction_from_left) {
        upper_square_x += arrow_width - square_size;
        lower_square_x += arrow_width - square_size;
        point_square_x -= arrow_width - square_size;
    }

    // draw upper square
    frame.drawRectangle(
        upper_square_x, 
        upper_square_y,
        upper_square_x + square_size,
        upper_square_y + square_size, 
        color);

    // draw lower square
    frame.drawRectangle(
        lower_square_x, 
        lower_square_y,
        lower_square_x + square_size,
        lower_square_y + square_size, 
        color);

    // draw point square
    frame.drawRectangle(
        point_square_x, 
        point_square_y,
        point_square_x + square_size,
        point_square_y + square_size, 
        color);
}
