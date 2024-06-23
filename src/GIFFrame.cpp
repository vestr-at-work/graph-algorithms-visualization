
#include "GIFFrame.hpp"
#include "gif.hpp"
#include <stdexcept>


GIFFrame::GIFFrame(std::size_t width, std::size_t height) 
    : m_width(width), m_height(height), m_frame(width, height) {}

void GIFFrame::drawRectangle(std::size_t left_upper_x, std::size_t left_upper_y,
        std::size_t right_lower_x, std::size_t right_lower_y, Color color) {
    if (left_upper_x > m_width || left_upper_x < 0
        || right_lower_x > m_width || right_lower_x < 0
        || left_upper_y > m_height || left_upper_y < 0
        || right_lower_y > m_height || right_lower_y < 0) {

            throw std::invalid_argument("Range outside frame");
        }

    for (int y = left_upper_y; y < right_lower_y; ++y) {
        for (int x = left_upper_x; x < right_lower_x; ++x) {
            m_frame[{y, x}] = gif::Pixel{
                color.R,
                color.G,
                color.B
            };
        }
    }
}

const gif::Frame& GIFFrame::getFrame() const {
    return m_frame;
}

std::size_t GIFFrame::width() const {
    return m_width;
}

std::size_t GIFFrame::height() const {
    return m_height;
}