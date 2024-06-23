#ifndef DrawHelpers_hpp
#define DrawHelpers_hpp

#include "Color.hpp"
#include "Frame.hpp"

void drawVerticalArrow(
    std::size_t pos_x, 
    std::size_t pos_y, 
    std::size_t arrow_width, 
    std::size_t arrow_height, 
    std::size_t square_size, 
    bool direction_from_top, 
    Frame& frame, 
    const Color& color
);

void drawHorizontalArrow(
    std::size_t pos_x, 
    std::size_t pos_y, 
    std::size_t arrow_width, 
    std::size_t arrow_height, 
    std::size_t square_size, 
    bool direction_from_left, 
    Frame& frame, 
    const Color& color
);

#endif